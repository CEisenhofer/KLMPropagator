using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using Microsoft.Z3;

// Reference Z3 library to use simplifier (makes KLMlean's life easier)

public class Benchmark {
    public enum Logic {
        C, CL, P,
    }
    // Example for generating benchmarks with 20 conditional pairs as premisses
    // [path-to-klmPropagator]
    // [path-to-prolog]
    // [path-to-KLM_C.pl]
    // [path-to-KLM_CL.pl]
    // [path-to-KLM_P.pl]
    // [path-to-NESCOND.pl]
    // [path-to-IOCondProver.pl]
    // 1 5 1 3 20
    public static int Main(string[] args) {
        int seed = 1;
        int maxVars = 10;
        int maxDepth = 2;
        int maxLength = 10;
        int maxNodes = 10;

        const int parseStart = 7;

        if (args.Length < parseStart) {
            Console.WriteLine("Needs at least three arguments: KLMPropagator.exe, PROLOG.exe, Path_to_KLM_C.pl Path_to_KLM_CL.pl Path_to_KLM_P.pl Path_to_NESCOND.pl Path_to_IOCondProver.pl");
            return -1;
        }
        string klmup = args[0];
        string sicstus = args[1];
        string klmlean_C = args[2];
        string klmlean_CL = args[3];
        string klmlean_P = args[4];
        string nescond = args[5];
        string ioCond = args[6];

        if (args.Length == 0) {
            Run(klmup, sicstus, klmlean_C, klmlean_CL, klmlean_P, nescond, ioCond);
            return 0;
        }
        // <seed>, <max_var>, <max_depth>, <max_len>, <max_nodes>
        if (args.Length >= parseStart + 1 && args[parseStart] != "0") {
            if (!int.TryParse(args[parseStart], out seed)) {
                Console.WriteLine("Cannot convert " + args[parseStart] + " to random seed");
                return -1;
            }
        }
        else {
            Console.WriteLine("Warning: No seed given -> Using time");
        }
        if (args.Length >= parseStart + 2) {
            if (!int.TryParse(args[parseStart + 1], out maxVars)) {
                Console.WriteLine("Cannot convert " + args[parseStart + 1] + " to maximum number of variables");
                return -1;
            }
        }
        else {
            Console.WriteLine("Warning: No number of variables given -> Using " + maxVars);
        }
        if (args.Length >= parseStart + 3) {
            if (!int.TryParse(args[parseStart + 2], out maxDepth)) {
                Console.WriteLine("Cannot convert " + args[parseStart + 2] + " to maximum depth");
                return -1;
            }
        }
        else {
            Console.WriteLine("Warning: No max depth given -> Using " + maxDepth);
        }
        if (args.Length >= parseStart + 4) {
            if (!int.TryParse(args[parseStart + 3], out maxLength)) {
                Console.WriteLine("Cannot convert " + args[parseStart + 3] + " to maximum length");
                return -1;
            }
        }
        else {
            Console.WriteLine("Warning: No max length given -> Using " + maxLength);
        }
        if (args.Length >= parseStart + 5) {
            if (!int.TryParse(args[parseStart + 4], out maxNodes)) {
                Console.WriteLine("Cannot convert " + args[parseStart + 4] + " to maximum nodes");
                return -1;
            }
        }
        else {
            Console.WriteLine("Warning: No max nodes given -> Using " + maxNodes);
        }

        foreach (var file in Directory.EnumerateFiles(".", ".smt2").Concat(Directory.EnumerateFiles(".", ".klm"))) {
            try {
                File.Delete(file);
            }
            catch {
                Console.WriteLine("Could not delete smtFile: " + file);
            }
        }
        if (maxLength > maxVars) {
            Console.WriteLine("Maximum variables smaller than maximal length; decreasing length");
            maxLength = maxVars;
        }

        Console.WriteLine("Seed: " + seed);
        RandomFormula randomFormula = new(seed, maxVars, maxDepth, maxLength, maxNodes);
        List<string> z3List = new();
        List<string> klmList_c = new();
        List<string> klmList_cl_p = new();
        List<string> nescondList = new();
        List<string> ioCondList = new();

        using Context ctx = new();
        using Params p = ctx.MkParams();
        p.Add("local_ctx", true);

        for (int i = 0; i < 100; i++) {
            Specification spec = randomFormula.gen_spec();

            string z3 = spec.ToZ3String(ctx, p);
            string klm_c = spec.ToKLMString(ctx, p, Logic.C);
            string klm_cl_p = spec.ToKLMString(ctx, p, Logic.CL);
            string nescond_file = spec.ToNESCONDString(ctx, p);
            string ioCond_file = spec.ToIOCondString(ctx, p);

            StringBuilder sb = new();

            for (int j = 0; j < randomFormula.CurrentVariables; j++) {
                sb.AppendLine("(declare-const v" + j + " Bool)");
            }
            sb.Append(z3);
            z3List.Add(sb.ToString());
            klmList_c.Add(klm_c);
            klmList_cl_p.Add(klm_cl_p);
            nescondList.Add(nescond_file);
            ioCondList.Add(ioCond_file);
        }

        for (int i = 0; i < z3List.Count; i++) {
            Console.WriteLine("Wrote bench" + i + ".(smt2/klm/nescond/iocond)");
            File.WriteAllText("bench" + i + ".smt2", z3List[i]);
            File.WriteAllText("bench" + i + ".c.klm", klmList_c[i]);
            File.WriteAllText("bench" + i + ".cl.p.klm", klmList_cl_p[i]);
            File.WriteAllText("bench" + i + ".nescond", nescondList[i]);
            File.WriteAllText("bench" + i + ".iocond", ioCondList[i]);
        }
        Run(klmup, sicstus, klmlean_C, klmlean_CL, klmlean_P, nescond, ioCond);

        for (int i = 0; i < 10; i++) // Avoid closing it by accident
            Console.ReadKey();
        return 0;
    }

    const int timeout = 10_000;

    static (bool worked, bool sat) RunKLMPropagator(string program, string inputFile, string args, ref int succ, ref int satCnt) {
        using Process proc = new();
        proc.StartInfo = new ProcessStartInfo {
            UseShellExecute = false,
            FileName = program,
            Arguments = args + " \"" + inputFile + "\"",
            RedirectStandardOutput = true,
            CreateNoWindow = true,
        };

        StringBuilder outputBuilder = new();
        proc.OutputDataReceived += (_, args) =>
        {
            outputBuilder.AppendLine(args.Data);
        };
        Stopwatch sw = Stopwatch.StartNew();
        bool worked = proc.Start();
        proc.BeginOutputReadLine();
        if (!worked) {
            Console.WriteLine("Could not start " + program);
            Environment.Exit(-1);
        }
        worked = proc.WaitForExit(timeout);
        sw.Stop();
        if (!worked)
            proc.Kill(true);
        else {
            succ++;
            if (proc.ExitCode != 0)
                throw new Exception(program + " crashed: " + proc.ExitCode);
        }
        string output = outputBuilder.ToString();
        bool sat = false;

        if (worked) {
            sat = output.Contains("\nsat in ");
            if (!sat && !output.Contains("\nunsat in ")) {
                worked = false;
                Console.WriteLine("No UP output");
            }
            else {
                if (sat)
                    satCnt++;
            }
        }
        
        Console.WriteLine("Done with " + inputFile + ": " + worked + " -> " + (!worked ? "?" : sat));

        return (worked, sat);
    }

    static (bool worked, bool sat) RunProlog(string prolog, string program, string inputFile, string unsatPattern, ref int succ, ref int satCnt) {
        string klmContent = File.ReadAllText(inputFile);
        using Process proc = new();
        proc.StartInfo = new ProcessStartInfo {
            UseShellExecute = false,
            FileName = prolog,
            Arguments = "-l \"" + program + "\"",
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            RedirectStandardInput = true,
            CreateNoWindow = true,
        };
        StringBuilder outputBuilder = new();
        StringBuilder errorBuilder = new();
        proc.OutputDataReceived += (_, args) =>
        {
            outputBuilder.AppendLine(args.Data);
        };
        proc.ErrorDataReceived += (_, args) =>
        {
            errorBuilder.AppendLine(args.Data);
        };
        Stopwatch klmSw = Stopwatch.StartNew();
        bool worked = proc.Start();
        proc.BeginOutputReadLine();
        proc.BeginErrorReadLine();
        if (!worked) {
            Console.WriteLine("Could not start KLMlean");
            Environment.Exit(-1);
        }

        proc.StandardInput.WriteLine(klmContent + "\n");
        proc.StandardInput.Close();
        worked = proc.WaitForExit(timeout);
        klmSw.Stop();
        Thread.Sleep(100);
        if (!worked)
            proc.Kill(true);
        else {
            succ++;
            if (proc.ExitCode != 0)
                throw new Exception(program + " crashed: " + proc.ExitCode);
        }
        string output = errorBuilder + "\n" + outputBuilder;
        bool sat = false;

        if (worked) {
            sat = output.TrimEnd().EndsWith("\nno") || output.TrimEnd().Contains("\nfalse.");
            if (!sat && !output.Contains(unsatPattern)) {
                worked = false;
                Console.WriteLine("No " + program + " output");
            }
            else {
                if (sat)
                    satCnt++;
            }
        }

        Console.WriteLine("Done with " + inputFile + ": " + worked + " -> " + (!worked ? "?" : sat));

        return (worked, sat);
    }

    static void RunC(
        string klmPropagator, string prolog, string klmLean, string nescond) {

        string location = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
        var files = Directory.EnumerateFiles(location, "*.smt2");

        int propagatorSucc = 0, leanSucc = 0, nescondSucc = 0;
        int propagatorSatCnt = 0, leanSatCnt = 0, nescondSatCnt = 0;

        Console.WriteLine("C:");

        foreach (var smtFile in files) {
            string klmFile = Path.Join(Path.GetDirectoryName(smtFile), Path.GetFileNameWithoutExtension(smtFile) + ".c.klm");
            if (!File.Exists(klmFile)) {
                Console.WriteLine("Skipping " + smtFile + " as there is no .c.klm file");
                return;
            }
            string nescondFile = Path.Join(Path.GetDirectoryName(smtFile), Path.GetFileNameWithoutExtension(smtFile) + ".nescond");
            if (!File.Exists(nescondFile)) {
                Console.WriteLine("Skipping " + smtFile + " as there is no .nescond file");
                return;
            }

            var propagatorRes = RunKLMPropagator(klmPropagator, smtFile, "-l=C --smtlib2 --no-check --no-model ", ref propagatorSucc, ref propagatorSatCnt);
            var leanRes =  RunProlog(prolog, klmLean, klmFile, "\nTree = tree(", ref leanSucc, ref leanSatCnt);
            var nescondRes = RunProlog(prolog, nescond, nescondFile, "\nRes = tree(", ref nescondSucc, ref nescondSatCnt);

            if (propagatorRes.worked && leanRes.worked && propagatorRes.sat != leanRes.sat) {
                Console.WriteLine("KLMPropagator/KLMlean different results");
            }
            if (propagatorRes.worked && nescondRes.worked && propagatorRes.sat != nescondRes.sat) {
                Console.WriteLine("KLMPropagator/NESCOND different results");
            }
        }

        Console.WriteLine("C:");
        Console.WriteLine($"KLMPropagator - Success: {propagatorSucc}; Sat: {propagatorSatCnt}; Unsat: {propagatorSucc - propagatorSatCnt}");
        Console.WriteLine($"KLMLean - Success: {leanSucc}; Sat: {leanSatCnt}; Unsat: {leanSucc - leanSatCnt}");
        Console.WriteLine($"NESCOND - Success: {nescondSucc}; Sat: {nescondSatCnt}; Unsat: {nescondSucc - nescondSatCnt}");
    }

    static void RunCL(
        string klmPropagator, string prolog, string klmLean) {

        string location = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
        var files = Directory.EnumerateFiles(location, "*.smt2");

        int propagatorSucc = 0, leanSucc = 0;
        int propagatorSatCnt = 0, leanSatCnt = 0;

        Console.WriteLine("CL:");

        foreach (var smtFile in files) {
            string klmFile = Path.Join(Path.GetDirectoryName(smtFile), Path.GetFileNameWithoutExtension(smtFile) + ".cl.p.klm");
            if (!File.Exists(klmFile)) {
                Console.WriteLine("Skipping " + smtFile + " as there is no .cl.p.klm file");
                return;
            }

            var propagatorRes = RunKLMPropagator(klmPropagator, smtFile, "-l=CL --smtlib2 --no-check --no-model ", ref propagatorSucc, ref propagatorSatCnt);
            var leanRes =  RunProlog(prolog, klmLean, klmFile, "\nTree = tree(", ref leanSucc, ref leanSatCnt);

            if (propagatorRes.worked && leanRes.worked && propagatorRes.sat != leanRes.sat) {
                Console.WriteLine("KLMPropagator/KLMlean different results");
            }
        }

        Console.WriteLine("CL:");
        Console.WriteLine($"KLMPropagator - Success: {propagatorSucc}; Sat: {propagatorSatCnt}; Unsat: {propagatorSucc - propagatorSatCnt}");
        Console.WriteLine($"KLMLean - Success: {leanSucc}; Sat: {leanSatCnt}; Unsat: {leanSucc - leanSatCnt}");
    }

    static void RunCM(
        string klmPropagator, string prolog, string ioCond) {

        string location = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
        var files = Directory.EnumerateFiles(location, "*.smt2");

        int propagatorSucc = 0, iocondSucc = 0;
        int propagatorSatCnt = 0, iocondSatCnt = 0;

        Console.WriteLine("CM:");

        foreach (var smtFile in files) {
            string ioCondFile = Path.Join(Path.GetDirectoryName(smtFile), Path.GetFileNameWithoutExtension(smtFile) + ".iocond");
            if (!File.Exists(ioCondFile)) {
                Console.WriteLine("Skipping " + smtFile + " as there is no .iocond file");
                return;
            }

            var propagatorRes = RunKLMPropagator(klmPropagator, smtFile, "-l=CM --smtlib2 --no-check --no-model ", ref propagatorSucc, ref propagatorSatCnt);
            var ioCondRes = RunProlog(prolog, ioCond, ioCondFile, "\nRes = node(", ref iocondSucc, ref iocondSatCnt);

            if (propagatorRes.worked && ioCondRes.worked && propagatorRes.sat != ioCondRes.sat) {
                Console.WriteLine("KLMPropagator/IOCondSolver different results");
            }
        }

        Console.WriteLine("CM:");
        Console.WriteLine($"KLMPropagator - Success: {propagatorSucc}; Sat: {propagatorSatCnt}; Unsat: {propagatorSucc - propagatorSatCnt}");
        Console.WriteLine($"IOCondSolver - Success: {iocondSucc}; Sat: {iocondSatCnt}; Unsat: {iocondSucc - iocondSatCnt}");
    }

    static void RunP(
        string klmPropagator, string prolog, string klmLean) {

        string location = Path.GetDirectoryName(Assembly.GetEntryAssembly().Location);
        var files = Directory.EnumerateFiles(location, "*.smt2");

        int propagatorSucc = 0, leanSucc = 0;
        int propagatorSatCnt = 0, leanSatCnt = 0;

        Console.WriteLine("P:");

        foreach (var smtFile in files) {
            string klmFile = Path.Join(Path.GetDirectoryName(smtFile), Path.GetFileNameWithoutExtension(smtFile) + ".cl.p.klm");
            if (!File.Exists(klmFile)) {
                Console.WriteLine("Skipping " + smtFile + " as there is no .cl.p.klm file");
                return;
            }

            var propagatorRes = RunKLMPropagator(klmPropagator, smtFile, "-l=P --smtlib2 --no-check --no-model ", ref propagatorSucc, ref propagatorSatCnt);
            var leanRes =  RunProlog(prolog, klmLean, klmFile, "\nTree = tree(", ref leanSucc, ref leanSatCnt);

            if (propagatorRes.worked && leanRes.worked && propagatorRes.sat != leanRes.sat) {
                Console.WriteLine("KLMPropagator/KLMlean different results");
            }
        }

        Console.WriteLine("P:");
        Console.WriteLine($"KLMPropagator - Success: {propagatorSucc}; Sat: {propagatorSatCnt}; Unsat: {propagatorSucc - propagatorSatCnt}");
        Console.WriteLine($"KLMLean - Success: {leanSucc}; Sat: {leanSatCnt}; Unsat: {leanSucc - leanSatCnt}");
    }

    static void Run(string klmPropagator, string prolog,
        string klmlean_C, string klmlean_CL, string klmlean_P, string nescond, string ioCond) {

        RunC(klmPropagator, prolog, klmlean_C, nescond);
        RunCL(klmPropagator, prolog, klmlean_CL);
        RunCM(klmPropagator, prolog, ioCond);
        RunP(klmPropagator, prolog, klmlean_P);
    }
}


class NodeSpecification {
    
    public Formula LHS { get; }
    public Formula RHS { get; }

    public NodeSpecification(Formula lhs, Formula rhs) {
        LHS = lhs;
        RHS = rhs;
    }

    public override bool Equals(object? other) {
        if (ReferenceEquals(null, other)) 
            return false;
        if (ReferenceEquals(this, other))
            return true;
        if (other is not NodeSpecification n)
            return false;
        return LHS.Equals(n.LHS) && RHS.Equals(n.RHS);
    }

    public override int GetHashCode() => 41 * LHS.GetHashCode() + RHS.GetHashCode();

    public override string ToString() => "(" + LHS + ", " + RHS + ")";

    public string ToZ3String() => $"(edge {LHS.ToZ3()} {RHS.ToZ3()})";
}

class Specification {
    public List<NodeSpecification> positive { get; }
    public NodeSpecification negative { get; }

    public Specification(List<NodeSpecification> positive, NodeSpecification negative) {
        this.positive = positive;
        this.negative = negative;
    }

    public override string ToString() {
        StringBuilder ss = new();
        if (positive.Count == 0)
            ss.Append("true");
        else
            ss.Append(positive[0]);
        for (int i = 1; i < positive.Count; i++) {
            ss.Append(", ").Append(positive[i]);
        }
        ss.Append(" => ").Append(negative);
        return ss.ToString();
    }

    public string ToZ3String(Context ctx, Params p) {

        StringBuilder ss = new();
        for (int i = 0; i < positive.Count; i++) {
            ss.AppendLine($"(assert (edge {positive[i].LHS.ToZ3Expr(ctx).Simplify(p).ToString()} {positive[i].RHS.ToZ3Expr(ctx).Simplify(p).ToString()}))");
        }
        ss.Append($"(assert (not (edge {negative.LHS.ToZ3Expr(ctx).Simplify(p).ToString()} {negative.RHS.ToZ3Expr(ctx).Simplify(p).ToString()})))");
        return ss.ToString();
    }
    public string ToKLMString(Context ctx, Params p, Benchmark.Logic logic) {
        StringBuilder ss = new();
        ss.Append("prove([");
        for (int i = 0; i < positive.Count; i++) {
            ss.Append(ToKLMString(positive[i].LHS.ToZ3Expr(ctx).Simplify(p))).Append(" => ").Append(ToKLMString(positive[i].RHS.ToZ3Expr(ctx).Simplify(p)));
            ss.Append(", ");
        }
        ss.Append("(neg (").Append(ToKLMString(negative.LHS.ToZ3Expr(ctx).Simplify(p))).Append(" => ").Append(ToKLMString(negative.RHS.ToZ3Expr(ctx).Simplify(p)));
        ss.AppendLine(logic == Benchmark.Logic.C ? "))], [], Tree, [])." : "))], [], Tree).\n");
        return ss.ToString();
    }

    static string ToKLMString(Expr e) {
        return e.FuncDecl.DeclKind switch {
            Z3_decl_kind.Z3_OP_UNINTERPRETED => e.FuncDecl.Name.ToString(),
            Z3_decl_kind.Z3_OP_AND => $"({string.Join(" and ", e.Args.Select(ToKLMString))})",
            Z3_decl_kind.Z3_OP_OR => $"({string.Join(" or ", e.Args.Select(ToKLMString))})",
            Z3_decl_kind.Z3_OP_IMPLIES => $"({ToKLMString(e.Arg(0))} -> {ToKLMString(e.Arg(1))})",
            Z3_decl_kind.Z3_OP_NOT => $"(neg {ToKLMString(e.Arg(0))})",
            Z3_decl_kind.Z3_OP_FALSE => "(f and (neg f))",
            Z3_decl_kind.Z3_OP_TRUE => "(t or (neg t))",
            _ => throw new ArgumentException("Conversion from " + e + " to KLM not implemented"),
        };
    }

    public string ToNESCONDString(Context ctx, Params p) {
        StringBuilder ss = new();
        ss.Append("prove([");
        var total = positive.Append(negative).ToList();
        ss.Append(
            string.Join(" -> ", total.Select(o =>
            "(" + ToNESCONDString(o.LHS.ToZ3Expr(ctx).Simplify(p)) + " => " +
            ToNESCONDString(o.RHS.ToZ3Expr(ctx).Simplify(p)) + ")")));
        ss.AppendLine("], [], Res).\n");
        return ss.ToString();
    }

    static string ToNESCONDString(Expr e) {
        return e.FuncDecl.DeclKind switch {
            Z3_decl_kind.Z3_OP_UNINTERPRETED => $"({e.FuncDecl.Name})",
            Z3_decl_kind.Z3_OP_AND => $"({string.Join(" ^ ", e.Args.Select(ToNESCONDString))})",
            Z3_decl_kind.Z3_OP_OR => $"({string.Join(" v ", e.Args.Select(ToNESCONDString))})",
            Z3_decl_kind.Z3_OP_IMPLIES => $"({ToNESCONDString(e.Arg(0))} -> {ToNESCONDString(e.Arg(1))})",
            Z3_decl_kind.Z3_OP_NOT => $"(!{ToNESCONDString(e.Arg(0))})",
            Z3_decl_kind.Z3_OP_FALSE => "false",
            Z3_decl_kind.Z3_OP_TRUE => "true",
            _ => throw new ArgumentException("Conversion from " + e + " to NESCOND not implemented"),
        };
    }

    public string ToIOCondString(Context ctx, Params p) {
        StringBuilder ss = new();
        ss.Append("prove(out3id, seq([");
        for (int i = 0; i < positive.Count; i++) {
            ss.Append("cimp(")
                .Append(ToIOCondString(positive[i].LHS.ToZ3Expr(ctx).Simplify(p)))
                .Append(", ")
                .Append(ToIOCondString(positive[i].RHS.ToZ3Expr(ctx).Simplify(p)))
                .Append(')');
            if (i + 1 < positive.Count)
                ss.Append(", ");
        }
        ss.Append("], [cimp(")
            .Append(ToIOCondString(negative.LHS.ToZ3Expr(ctx).Simplify(p)))
            .Append(", ")
            .Append(ToIOCondString(negative.RHS.ToZ3Expr(ctx).Simplify(p)))
            .Append(")]), Res).\n");
        return ss.ToString();
    }

    static string ToIOCondString(Expr e) {
        switch (e.FuncDecl.DeclKind) {
            case Z3_decl_kind.Z3_OP_UNINTERPRETED:
                return $"at({e.FuncDecl.Name})";
            case Z3_decl_kind.Z3_OP_AND:
                if (e.NumArgs == 1)
                    return ToIOCondString(e.Arg(0));
                return $"and({ToIOCondString(e.Arg(0))}, {ToIOCondString(e.Context.MkAnd(e.Args.Skip(1).OfType<BoolExpr>()))})";
            case Z3_decl_kind.Z3_OP_OR:
                if (e.NumArgs == 1)
                    return ToIOCondString(e.Arg(0));
                return $"or({ToIOCondString(e.Arg(0))}, {ToIOCondString(e.Context.MkOr(e.Args.Skip(1).OfType<BoolExpr>()))})";
            case Z3_decl_kind.Z3_OP_IMPLIES:
                throw new NotImplementedException();
            case Z3_decl_kind.Z3_OP_NOT:
                return $"neg({ToIOCondString(e.Arg(0))})";
            case Z3_decl_kind.Z3_OP_FALSE:
                return "and(at(f), neg(at(f)))";
            case Z3_decl_kind.Z3_OP_TRUE:
                return "or(at(t), neg(at(t)))";
            default:
                throw new ArgumentException("Conversion from " + e + " to IOCond not implemented");
        }
    }
}

class RandomFormula {

    public int CurrentVariables { get; private set; }

    readonly Random random;
    readonly int maxVars;
    readonly int maxDepth;
    readonly int maxLength;
    readonly int maxNodes;
    public RandomFormula(int seed, int maxVars, int maxDepth, int maxLength, int maxNodes) {
        this.maxVars = maxVars;
        this.maxDepth = maxDepth;
        this.maxLength = maxLength;
        this.maxNodes = maxNodes;
        random = new Random(seed);
    }

    public Formula gen_random() {

        int res = random.Next(5);
        if (res == 0)
            return gen_literal();
//        if (res is >= 1 and <= 2)
//            return GenAnd(0);
//        if (res is >= 3 and <= 4)
//            return GenOr(0);
        if (res is >= 1 and <= 4)
            return GenOr(0);
        throw new Exception();
    }

    public Formula gen_literal() {
        bool neg = random.Next(0, 2) == 0;
        var variable = random.Next(maxVars);
        if (variable >= CurrentVariables)
            variable = CurrentVariables++;
        Formula literal = new Atom(variable);
        if (neg)
            literal = new Negation(literal);
        return literal;
    }

    Formula GenAnd(int depth) {
        if (depth >= maxDepth)
            return gen_literal();

        int length = 2 + random.Next(maxLength - 1);
        Formula[] args = new Formula[length];
        HashSet<Atom> atoms = new();
        for (int i = 0; i < length; i++) {
            if (random.Next(3) == 0) {
                Atom atom;
                do {
                    args[i] = gen_literal();
                    if (args[i] is Negation n)
                        atom = (Atom)n.formula;
                    else
                        atom = (Atom)args[i];
                } while (atoms.Contains(atom));
                atoms.Add(atom);
            }
            else
                args[i] = GenOr(depth + 1);
        }
        return new And(args);
    }

    Formula GenOr(int depth) {
        if (depth >= maxDepth)
            return gen_literal();

        int length = 2 + random.Next(maxLength - 1);
        Formula[] args = new Formula[length];
        HashSet<Atom> atoms = new();
        for (int i = 0; i < length; i++) {
            if (random.Next(3) == 0) {
                Atom atom;
                do {
                    args[i] = gen_literal();
                    if (args[i] is Negation n)
                        atom = (Atom)n.formula;
                    else
                        atom = (Atom)args[i];
                } while (atoms.Contains(atom));
                atoms.Add(atom);
            }
            else
                args[i] = GenAnd(depth + 1);
        }
        return new Or(args);
    }

    public Specification gen_spec() {
        CurrentVariables = 0;
        //int nodes = 1 + random.Next(maxNodes);
        int nodes = maxNodes;
        HashSet<NodeSpecification> added = new();
        List<NodeSpecification> positive = new();
        NodeSpecification? nodeSpecification;

        for (int i = 0; i < nodes; i++) {
            do {
                var lhs = gen_random();
                var rhs = gen_random();
                nodeSpecification = new NodeSpecification(lhs, rhs);
            } while (added.Contains(nodeSpecification));
            added.Add(nodeSpecification);
            positive.Add(nodeSpecification);
        }
        do {
            var lhs = gen_random();
            var rhs = gen_random();
            nodeSpecification = new NodeSpecification(lhs, rhs);
        } while (added.Contains(nodeSpecification));
        added.Add(nodeSpecification);

        return new Specification(positive, nodeSpecification);
    }
};

public abstract class Formula {

    public abstract BoolExpr ToZ3Expr(Context ctx);
    public abstract string ToZ3();
    public abstract string ToKLM();
    public abstract string ToNESCOND();

    public abstract override bool Equals(object? other);

    public abstract override int GetHashCode();

    public abstract override string ToString();
};

class Atom : Formula {
    public int Id { get; }
    
    public Atom(int id) {
        Id = id;
    }

    public override BoolExpr ToZ3Expr(Context ctx) => ctx.MkBoolConst("v" + Id);

    public override string ToZ3() => "v" + Id;

    public override string ToKLM() => "v" + Id;

    public override string ToNESCOND() => "v" + Id;

    public override bool Equals(object? other) => other is Atom a && Id == a.Id;

    public override int GetHashCode() => Id;

    public override string ToString() => "v" + (Id + 1);
};

class Negation : Formula {
    
    public Formula formula { get; }

    public Negation(Formula formula) {
        this.formula = formula;
    }

    public override BoolExpr ToZ3Expr(Context ctx) => ctx.MkNot(formula.ToZ3Expr(ctx));

    public override string ToZ3() => $"(not {formula.ToZ3()})";

    public override string ToKLM() => $"(neg {formula.ToKLM()})";

    public override string ToNESCOND() => $"(!{formula.ToNESCOND()})";

    public override bool Equals(object? other) => other is Negation n && formula.Equals(n.formula);

    public override int GetHashCode() {
        return 176953 * formula.GetHashCode();
    }

    public override string ToString() {
        return "!" + formula;
    }
}

class And : Formula {

    public Formula[] args { get; }

    public And(Formula[] args) {
        this.args = args;
    }

    public override BoolExpr ToZ3Expr(Context ctx) => ctx.MkAnd(args.Select(o => o.ToZ3Expr(ctx)));

    public override string ToZ3() => $"(and {string.Join(' ', args.Select(o => o.ToZ3()))})";

    public override string ToKLM() => $"({string.Join(" and " , args.Select(o => o.ToKLM()))})";

    public override string ToNESCOND() => $"({string.Join(" ^ ", args.Select(o => o.ToNESCOND()))})";

    public override bool Equals(object? other) => other is And and && and.args.SequenceEqual(args);

    public override int GetHashCode() => args.Aggregate(19, (v, f) => v * 23 + f.GetHashCode());

    public override string ToString() {
        if (args.Length == 0)
            return "true";
        if (args.Length == 1)
            return args[0].ToString();
        StringBuilder sb = new StringBuilder();
        sb.Append('(').Append(string.Join(" && ", args.Select(o => o.ToString()))).Append(')');
        return sb.ToString();
    }
}

class Or : Formula {

    public Formula[] args { get; }

    public Or(Formula[] args) {
        this.args = args;
    }

    public override BoolExpr ToZ3Expr(Context ctx) => ctx.MkOr(args.Select(o => o.ToZ3Expr(ctx)));

    public override string ToZ3() => $"(or {string.Join(' ', args.Select(o => o.ToZ3()))})";

    public override string ToKLM() => $"({string.Join(" or ", args.Select(o => o.ToKLM()))})";

    public override string ToNESCOND() => $"({string.Join(" v ", args.Select(o => o.ToNESCOND()))})";

    public override bool Equals(object? other) => other is Or or && or.args.SequenceEqual(args);
    public override int GetHashCode() => args.Aggregate(21, (v, f) => v * 27 + f.GetHashCode());

    public override string ToString() {
        if (args.Length == 0)
            return "false";
        if (args.Length == 1)
            return args[0].ToString();
        StringBuilder sb = new StringBuilder();
        sb.Append('(').Append(string.Join(" || ", args.Select(o => o.ToString()))).Append(')');
        return sb.ToString();
    }
}
