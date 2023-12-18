#include "Program.h"
#include "LanguageLexer.h"
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <filesystem>

class LanguageVisitorImpl;

static decltype(std::chrono::high_resolution_clock::now()) start_time;
static decltype(std::chrono::high_resolution_clock::now()) end_time;

static void start_watch() {
    start_time = std::chrono::high_resolution_clock::now();
}

static uint64_t stop_watch() {
    end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
}

static std::unordered_map<std::string, Logic> SupportedLogicsMap;

static void crash_params() {
    std::cout << "Usage: KLM [-t=[timeout in ms] -l=[C|CL|CM|P|M] --smtlib --visual] <file>" << std::endl;
    exit(-1);
}

static void
parse_params(const std::vector<std::string> &args, unsigned &timeout, Logic &logic, bool &smtlib, bool &visual,
             bool &bench) {
    if (args.empty() || !std::filesystem::exists(args[args.size() - 1]))
        crash_params();

    SupportedLogicsMap["C"] = C;
    SupportedLogicsMap["CL"] = CL;
    SupportedLogicsMap["CM"] = CM;
    SupportedLogicsMap["P"] = P;
    SupportedLogicsMap["M"] = M;

    timeout = 0;
    logic = C;
    smtlib = false;
    visual = false;
    bench = false;

    for (int i = 0; i < args.size() - 1; i++) {
        if (starts_with(args[i], "-t=")) {
            std::stringstream ss(args[i]);
            ss.ignore(3);
            ss >> timeout;
            if (ss.fail())
                crash_params();
            if (timeout <= 0)
                crash_params();
            continue;
        }
        if (starts_with(args[i], "-l=")) {
            const std::string logicString = args[i].substr(3);
            auto it = SupportedLogicsMap.find(logicString);
            if (it == SupportedLogicsMap.end())
                crash_params();
            logic = it->second;
            continue;
        }
        if (args[i] == "--bench") {
            bench = true;
        }
        if (args[i] == "--smtlib") {
            smtlib = true;
        }
        if (args[i] == "--visual") {
            visual = true;
        }
    }
}

int main(int argc, char** argv) {
    std::vector<std::string> args(argc);
    for (unsigned i = 0; i < argc; i++) {
        args[i] = argv[i];
    }
    unsigned timeout;
    Logic logic;
    bool smtlib;
    bool visual;
    bool bench;
    parse_params(args, timeout, logic, smtlib, visual, bench);

    if (bench) {
        benchmark(logic);
        return 0;
    }

    context ctx;
    std::string smtlib2;
    std::string content;
    try {
        std::ifstream t(args[args.size() - 1]);
        std::stringstream buffer;
        buffer << t.rdbuf();
        content = buffer.str();
    }
    catch (std::exception &e) {
        std::cout << "Could not open file: " << args[0] << std::endl;
        exit(-1);
    }
    if (smtlib) {
        smtlib2 = content;
    } else {
        antlr4::ANTLRInputStream inputStream(content);
        LanguageLexer lexer(&inputStream);
        antlr4::CommonTokenStream commonTokenStream(&lexer);
        LanguageParser parser(&commonTokenStream);
        LanguageVisitorImpl visitor(ctx);
        expr commands(ctx);
        try {
            commands = std::any_cast<expr>(parser.statements()->accept(&visitor));
        }
        catch (std::exception &e) {
            exit(-1);
        }
        expr_vector assertions(ctx);
        if (commands.is_and())
            assertions.push_back(commands);
        else {
            for (const auto &arg: commands.args()) {
                assertions.push_back(arg);
            }
        }
        smtlib2 = to_SMTLIB(assertions);
    }
    Solve(ctx, smtlib2, timeout, logic, smtlib, visual);
    return 0;
}

void
Solve(context &context, const std::string &smtlib2, unsigned timeout, Logic logic, bool smtlibOutput, bool visual) {
    sort nodeSort = context.uninterpreted_sort("Node");

    sort_vector domain(context);
    domain.push_back(context.bool_sort());
    domain.push_back(context.bool_sort());
    func_decl nodeFct = context.function("nodeReal", domain, context.bool_sort());

    domain.resize(0);
    domain.push_back(nodeSort);
    domain.push_back(nodeSort);
    func_decl nodeFctAbstr = context.user_propagate_function(
            context.str_symbol("node"), domain, context.bool_sort());

    sort_vector empty_sorts(context);
    func_decl_vector node_decl(context);
    node_decl.push_back(nodeFctAbstr);

    Z3_symbol decl_names[1];
    decl_names[0] = Z3_mk_string_symbol(context, "node");
    Z3_func_decl decl_fcts[1];
    decl_fcts[0] = nodeFct;

    auto parsed = Z3_parse_smtlib2_string(context, smtlib2.c_str(), 0, nullptr, nullptr, 1, decl_names, decl_fcts);
    if (Z3_get_error_code(context) != Z3_error_code::Z3_OK)
        throw z3::exception(Z3_get_error_msg(context, Z3_get_error_code(context)));

    expr assertion = mk_and(expr_vector(context, parsed));

    std::unordered_map<expr, expr_template*, expr_hash, expr_eq> abstraction;
    std::unordered_map<expr, std::optional<expr>, expr_hash, expr_eq> abstractionInv;

    assertion = replace_nodes(assertion, nodeFct, nodeFctAbstr, nodeSort, abstraction, abstractionInv);

    params param(context);
    param.set("smt.phase_selection", 1u);
    param.set("solver.timeout", timeout);

    solver solver(context);
    solver.set(param);

    start_watch();
    KLMPropagator propagator(solver, nodeFctAbstr, logic, abstraction);
    std::cout << assertion << std::endl;
    solver.add(assertion);

    const check_result res = solver.check();
    const auto time = stop_watch();
    std::cout << res << " in " << time << "mrs" << std::endl;
    if (res == sat) {
        std::cout << std::endl;
        std::string model = propagator.get_model(smtlibOutput);
        std::cout << model << std::endl;
        if (visual) {
            propagator.display_model(smtlibOutput);
            std::ofstream of("output-info.txt");
            of << model;
            of.close();
        }
        propagator.check_model();
    }
}


void get_constants(expr e, std::unordered_set<expr, expr_hash, expr_eq> &names) {
    if (e.decl().decl_kind() == Z3_OP_UNINTERPRETED && e.num_args() == 0) {
        names.insert(e);
        return;
    }
    for (const auto &arg: e.args()) {
        get_constants(arg, names);
    }
}

std::string to_SMTLIB(const expr_vector &assertions) {
    expr conj = assertions.size() == 1 ? assertions[0] : mk_and(assertions);
    std::unordered_set<expr, expr_hash, expr_eq> constants;
    get_constants(conj, constants);
    std::stringstream sb;
    for (const auto &constant: constants) {
        sb << "(declare-const " << constant.decl().name().str() << " Bool)\n";
    }

    for (const auto &assertion: assertions) {
        sb << "(assert " << assertion << ")\n";
    }
    return sb.str();
}

void benchmark(Logic logic) {
    std::random_device rd;
    int seed = 0;
    std::cout << "Random seed: " << seed << std::endl;
    constexpr int maxVariables = 10;
    RandomFormula randomFormula(seed, maxVariables, 2, 10, 10);
    for (int i = 0; i < 100; i++) {
        context ctx;
        expr_vector assertions(ctx);
        Specification spec = randomFormula.gen_spec();
        sort_vector domain(ctx);
        domain.push_back(ctx.bool_sort());
        domain.push_back(ctx.bool_sort());
        func_decl nodeFct = ctx.function("node", domain, ctx.bool_sort());
        for (const auto &pos: spec.get_positive()) {
            auto lhs = pos->get_lhs();
            auto rhs = pos->get_rhs();
            assertions.push_back(nodeFct(lhs->ToZ3(ctx).simplify(), rhs->ToZ3(ctx).simplify()));
        }
        assertions.push_back(
                !nodeFct(spec.get_negative()->get_lhs()->ToZ3(ctx).simplify(),
                         spec.get_negative()->get_rhs()->ToZ3(ctx).simplify()));
        Solve(ctx, to_SMTLIB(assertions), 0, logic, false, false);
    }
}

expr create_template(expr e, std::unordered_map<std::string, std::pair<unsigned, std::optional<expr>>> &variables) {
    if (e.decl().decl_kind() == Z3_OP_UNINTERPRETED && e.num_args() == 0 && e.is_bool()) {
        std::string name = e.decl().name().str();
        const auto it = variables.find(name);
        if (it != variables.end())
            return *it->second.second;
        expr varRes = e.ctx().variable((unsigned) variables.size(), e.ctx().bool_sort());
        variables[name] = std::make_pair(variables.size(), varRes);
        return varRes;
    }
    expr_vector args(e.ctx());
    for (unsigned i = 0; i < e.num_args(); i++) {
        args.push_back(create_template(e.arg(i), variables));
    }
    return e.decl()(args);
}

expr replace_nodes(const expr &e, const func_decl &oldDecl, const func_decl &newDecl, const sort &nodeSort,
                   std::unordered_map<expr, expr_template*, expr_hash, expr_eq> &abstraction,
                   std::unordered_map<expr, std::optional<expr>, expr_hash, expr_eq> &abstractionInv) {
    if (Z3_is_eq_func_decl(e.ctx(), e.decl(), oldDecl)) {
        expr arg1 = e.arg(0).simplify();
        expr arg2 = e.arg(1).simplify();
        auto it = abstractionInv.find(arg1);
        expr abstr1(e.ctx()), abstr2(e.ctx());

        if (it == abstractionInv.end()) {
            abstr1 = e.ctx().constant(arg1.to_string().c_str(), nodeSort);
            abstractionInv[arg1] = abstr1;
            std::unordered_map<std::string, std::pair<unsigned, std::optional<expr>>> variables;
            const auto templateExpr = create_template(arg1, variables);
            std::vector<std::string> variableNames = std::vector<std::string>(variables.size());
            for (const auto &variable: variables) {
                variableNames[variable.second.first] = variable.first;
            }
            abstraction[abstr1] = new expr_template(templateExpr, std::move(variableNames));
        } else
            abstr1 = *it->second;

        it = abstractionInv.find(arg2);

        if (it == abstractionInv.end()) {
            abstr2 = e.ctx().constant(arg2.to_string().c_str(), nodeSort);
            abstractionInv[arg2] = abstr2;
            std::unordered_map<std::string, std::pair<unsigned, std::optional<expr>>> variables;
            auto templateExpr = create_template(arg2, variables);
            std::vector<std::string> variableNames = std::vector<std::string>(variables.size());
            for (const auto &variable: variables) {
                variableNames[variable.second.first] = variable.first;
            }
            abstraction[abstr2] = new expr_template(templateExpr, std::move(variableNames));
        } else
            abstr2 = *it->second;

        return newDecl(abstr1, abstr2);
    }
    expr_vector args(e.ctx());
    for (const auto &arg: e.args()) {
        args.push_back(replace_nodes(arg, oldDecl, newDecl, nodeSort, abstraction, abstractionInv));
    }
    return e.decl()(args);
}
