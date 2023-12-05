import re
from test_runtime import ErrorType, test, print_exit

# fuck you python
true = True
false = False


def test_valid(code):
    test(code, "")


def test_invalid(code, expected_return_code):
    test(code, "", expected_return_code=expected_return_code)


def test_expr(code, result):
    code = f"""
        let result = {code}
        write(result)
    """
    test(code, result)


def test_expr_valid(code):
    code = f"""
        let result = {code}
    """
    test(code, "")


def test_expr_invalid(code):
    code = f"""
        let result = {code}
    """
    test(code, "", expected_return_code=ErrorType.error_type)


def hexfloat(value: float):
    # compensation for differences in printf and .hex()
    first_sub = re.sub("\\.0*p", "p", value.hex())
    return re.sub("0*p", "p", first_sub)


def test_all():
    error_call = ErrorType.error_call
    error_other_semantic = ErrorType.error_other_semantic
    error_type = ErrorType.error_type

    # variable declarations, definitions, assignments

    test("""
    let a = 5 // so it begins
    write(a)
    """, "5")
    test_valid("""
    var a: Int? = 5
    """)
    test_valid("""
    var a: Int? = nil
    """)

    test_valid("""
    var a: Int? = 5 + 5
    """)

    test_valid("""
    var a: Int? = 5
    var b: Int? = a
    """)

    test_valid("""
    let a: Int? = 5
    var b: Int? = a
    """)

    test_valid("""
    var a: Int? = 5
    let b: Int? = a
    """)

    test_valid("""
    let a: Int? = 5
    let b: Int? = a
    """)

    test_valid("""
    let a: Int = 5
    let b: Int? = a
    """)

    test("""
    let a: Int = 5
    let b: Int? = a - 7
    write(b)
    """, "-2")

    test("""
    let a: Int = 5
    let b: Int = a - 7
    write(b)
    """, "-2")

    test_valid("""
    var a: Int?
    write(a)
    """)

    test_valid("""
    var a: Int? = 5 - 7
    """)

    test_invalid("""
    let a = nil
    """, ErrorType.error_type_inference)

    test_invalid("""
    let a: Int = nil
    """, error_type)

    test_invalid("""
    let a: Int = 5.6
    """, error_type)

    test_invalid("""
    let a: Int = "kentus"
    """, error_type)

    test_invalid("""
    let a: Int = 4 > 5
    """, error_type)

    test_invalid("""
    let a = 4
    let b: Double = a
    """, error_type)

    test("""
    let a: Double? = 5
    write(a)
    """, hexfloat(5.0))

    test_valid("""
    let a: Double? = nil
    """)

    error_undefined_var = ErrorType.error_undefined_var

    test_invalid("""
    let a = b
    """, error_undefined_var)

    test_invalid("""
    let a: Int = b
    """, error_undefined_var)

    test_invalid("""
    let a: Int
    let b = a
    """, error_undefined_var)

    test_invalid("""
    let a: Int
    let b: Int = a
    """, error_undefined_var)

    test_invalid("""
    func a(_ a: Int) {}
    let a: Int
    a(a)
    """, error_undefined_var)

    test_invalid("""
    let a: Int
    write(a)
    """, error_undefined_var)

    # test_valid("""
    # let a: Int
    # if true {
    #     a = 5
    # } else { }
    # let b = a
    # """)

    # test_valid("""
    # let a: Int
    # var t = true
    # while t {
    #     a = 5
    #     t = false
    # }
    # let b = a
    # """)

    # test_invalid("""
    # var t: Bool
    # while t {
    # }
    # """, error_undefined_var)

    # test_invalid("""
    # var t: Bool
    # if t {
    # }
    # """, error_undefined_var)

    test("""
    let a: Double
    a = 5
    write(a)
    """, hexfloat(5.0))

    test_invalid("""
    let a: Double? // already initialized to nil
    a = 5
    write(a)
    """, error_other_semantic)

    test("""
    var a: Double? 
    a = 5
    write(a)
    """, hexfloat(5.0))

    test("""
    let a: Int
    let b: Int
    a = 5
    b = a
    write(b)
    """, "5")

    test("""
    let a: Int
    let b: Int
    a = 5
    b = a
    write(b)
    """, "5")

    test_invalid("""
    let a: Int
    let b: Int
    a = 5
    b = a
    a = 5
    """, error_other_semantic)

    test_invalid("""
    let a: Int
    let b: Int
    a = 5
    b = a
    b = 5
    """, error_other_semantic)

    test_invalid("""
    let a: Int
    a = "kentus"
    """, error_type)

    test_invalid("""
    let a: Int
    a = 4.5
    """, error_type)

    error_ident = ErrorType.error_ident

    test_invalid("""
    let a = 5
    let a = "helo"
    """, error_ident)

    test_invalid("""
    var a = 5
    let a = "helo"
    """, error_ident)

    test_invalid("""
    let a = 5
    var a : Int
    """, error_ident)

    test_invalid("""
    var a :Int
    var a = 4
    """, error_ident)

    # test("""
    # let a = 1
    # if true {
    #     write(a)
    #     let a = "2"
    #     if true {
    #         write(a)
    #         let a = 3
    #         write(a)
    #     } else {}
    # } else {}
    # """, "123")

    # if, if let, while statements

    # test("""
    # let a = 45 > 3
    # if a {
    #     write(a)
    # } else {}
    # """, "true")

    # test_invalid("""
    # let a: Bool? = 45 > 3
    # if a {
    #     write(a)
    # } else {}
    # """, error_type)

    test_invalid("""
    let a = 45
    if a {
        write(a)
    } else {}
    """, error_type)

    test_invalid("""
    if 45 + 45.0 {
        write(a)
    } else {}
    """, error_type)

    # test("""
    # if 45 > 3 {
    #     write(true)
    # } else {}
    # """, "true")

    test("""
    let a = 45 > 3
    let b = a == a
    if b {
        write(a)
    } else {}
    """, "true")

    test_invalid("""
    let a = 45 > 3
    if a {
        let kentus = "blentus"
    } else {
        write(kentus)
    }
    """, ErrorType.error_undefined_var)

    test("""
    let a: Int? = 45
    if let a {
        let kentus = a + 4
        let b: Int = a
        write(b)
    } else {}
    """, "45")

    test("""
    let a: Int? = 5
    if let a {
        let a = "shadowing"
        write(a)
    } else {
    }
    """, "shadowing")

    # test("""
    # let a: Int?
    # if let a {
    #     let kentus = a + 4
    #     let b: Int = a
    #     write(b)
    # } else {
    #     write(false)
    # }
    # let b = 5
    # """, "false")

    test_invalid("""
    if let a {
    } else { }
    """, ErrorType.error_undefined_var)

    test_invalid("""
    if let 5 {
    } else {
    }
    """, ErrorType.error_parser)

    test_invalid("""
    if let nil {
    } else {
    }
    """, ErrorType.error_parser)

    test("""
    var a = 5
    while a >= 0 {
        write(a)
        a = a - 1
    }
    """, "543210")

    test_invalid("""
    var a = 5
    while 4 {
        write(a)
        a = a - 1
    }
    """, error_type)

    test("""
    var a = 5
    while a >= 5 {
        write(a)
        a = a - 1
    }
    """, "5")

    # test("""
    # func a() {
    #     while true {
    #         var a = 5
    #         write(a)
    #         return
    #     }
    # }
    # a()
    # """, "5")

    # test("""
    # func a() {
    #     var t = true
    #     while t {
    #         var a = 5
    #         write(a)
    #         t = false
    #     }
    # }
    # a()
    # """, "5")

    # test("""
    # var t = true
    # func a() {
    #     while t {
    #         var a = 5
    #         write(a)
    #         t = false
    #     }
    # }
    # a()
    # """, "5")

    test("""
    var t = 5
    func a() {
        while t > 0 {
            var a = 5
            write(t)
            t = t - 1
        }
    }
    a()
    """, "54321")

    # function definitions, calls, returns

    test("""
    f1()

    func f1() {
        let a = 4
        write("a=", a )
    }
    """, "a=4")

    test("""
    f1()
    write(" ")
    f1()

    func f1() {
        let a = 4
        write("a=", a )
    }
    """, "a=4 a=4")

    test("""
    var a = 0
    f1()
    write(" ")
    f1()
    write(" ")
    f1()

    func f1() {
        write("a=", a )
        a = a + 1
    }
    """, "a=0 a=1 a=2")

    # test_invalid("""
    # if true {
    #     func a() {}
    # } else {}
    # """, error_other_semantic)

    test_invalid("""
    func a() {
        func b() {}
    }
    """, error_other_semantic)

    test_invalid("""
    func a() {
        func a() {}
    }
    """, error_other_semantic)

    test_invalid("""
    func a() {}
    func a() {}
    """, error_other_semantic)

    test_invalid("""
    let a = 5
    a()
    """, ErrorType.error_ident)

    test("""
    var a = 1
    var b: Int?
    func copy() {
        b = a
    }
    copy()
    write(b, " ")

    a = 2
    copy()
    write(b)
    """, "1 2")

    # parameter name must differ from identifier (why????)
    test_invalid("func abc(a a: Int) {}", error_other_semantic)

    test("""
    // mutual recursion

    func a(_ i: Int) {
        write("a: ", i, " ")
        let decrement = i - 1

        if decrement >= 0 {
            b(decrement)
        } else {}
    }

    func b(_ i: Int) {
        write("b: ", i, " ")
        let decrement = i - 1

        if decrement >= 0 {
            a(decrement)
        } else {}
    }

    a(4)
    b(4)
    """, "a: 4 b: 3 a: 2 b: 1 a: 0 b: 4 a: 3 b: 2 a: 1 b: 0 ")

    test_invalid("""
    func a(_ a: Int) {}
    a()
    """, error_call)

    test_invalid("""
    func a(_ a: Int) {}
    a(nil)
    """, error_call)

    test_invalid("""
    func a(_ a: Int) {}
    let value: Int? = 4
    a(value)
    """, error_call)

    test_invalid("""
    func a(_ a: Int) {}
    a("abc")
    """, error_call)

    test_invalid("""
    func a(_ a: Int) {}
    let value = 4.5
    a(value)
    """, error_call)

    test("""
    func concat2(what a: String, with b: String) -> String { return a + b }
    let unnamed = "abc" + "def"
    let named = concat2(what: "abc", with: "def")
    write(named, unnamed)
    """, "abcdefabcdef")

    test("""
    func a(_ a: Int) { write(a)}
    let value = 4
    a(value)
    """, "4")

    test_valid("""
    func a() { a() } // :wicked:
    """)

    test_valid("""
    func a() { c() }
    func c() { b() }
    func b() { a() }
    """)

    test_valid("""
    func a(_ i: Int, param j: Double?) { c(i, param: j) }
    func c(_ i: Int, param j: Double?) { b(i, param: j) }
    func b(_ i: Int, param j: Double?) { a(i, param: j) }
    """)

    test_valid("""
    func a(_ i: Int) { c(i) }
    func c(_ i: Int) { b(i) }
    func b(_ i: Int) { a(i) }
    """)

    test_valid("""
    func a(_ i: Int?) {}
    a(0)
    """)

    test_valid("""
    func a(_ i: Int?) {}
    a(nil)
    """)

    test_invalid("""
    func a(_ i: Int?) {}
    a(4.5)
    """, error_call)

    test_invalid("""
    func a(_ i: Int?) {}
    a(4, 5)
    """, error_call)

    test_invalid("""
    func a(_ i: Int?) {}
    a()
    """, error_call)

    test_valid("""
    func a(_ i: Int?) {}
    let a: Int? = 4
    a(a)
    """)

    test_valid("""
    func a(_ i: Int?) {}
    let a: Int?
    a(a)
    """)

    test_invalid("""
    func a(_ i: Int) {}
    a(nil)
    """, error_call)

    test_valid("""
    func a(i _: Int?) {}
    a(i: nil)
    """)

    test_valid("""
    func a(i _: Int?) {}
    a(i: 4)
    """)

    test_valid("""
    func a(i _: Int) {}
    a(i: 4)
    """)

    test_invalid("""
    func a(i _: Int) {}
    a(i: nil)
    """, error_call)

    test_valid("""
    func a(_ _: Int) {}
    a(4)
    """)

    test_invalid("""
    func a(_ _: Int) {}
    a(nil)
    """, error_call)

    test_invalid("""
    func a(_ _: Int) {}
    a(4.5)
    """, error_call)

    test("""
    func a(_ i: Int) {
        write(i, " ")
        let i = "kentus"
        write(i)
    }
    a(42)
    """, "42 kentus")

    test_invalid("""
    func a(kentus i: Int) { }
    a(42)
    """, error_other_semantic)

    test_invalid("""
    func a(_ i: Int) { }
    a(kentus: 42)
    """, error_other_semantic)

    test_invalid("""
    func a(_ i: Int) { }
    var b: Int = 5
    b = a(42)
    """, ErrorType.error_type)

    test_invalid("""
    func a(_ i: Int) { }
    let b = a(42)
    """, ErrorType.error_type)

    # return evaluation

    test_invalid("""
    func a() {
        return
        return 4
    }
    """, ErrorType.error_invalid_return)

    test_invalid("""
    func a() -> Int? {
        return
    }
    """, ErrorType.error_invalid_return)

    test_invalid("""
    func a() {
        return nil
    }
    """, ErrorType.error_invalid_return)

    test_invalid("""
    func a() -> Int {
        return 5
        return
    }
    """, ErrorType.error_invalid_return)

    # test_invalid("""
    # func a() -> Int {
    #     if true {
    #         return "kentus"
    #     } else {}
    # }
    # """, error_call)

    test_invalid("""
    func a() -> Int {
        return "kentus"
    }
    let a = a()
    write(a)
    """, error_call)

    # test_invalid("""
    # func a() -> String {
    #     while true {
    #         return "kentus"
    #     }
    # }
    # let a = a()
    # write(a)
    # """, error_other_semantic)

    # test("""
    # func a() -> Int {
    #     if true {
    #         let a = 5
    #         if true { return 1
    #         } else { return 5}
    #     } else {
    #         return 2
    #     }
    #
    #     while true {
    #         if true { return 3
    #         } else { return 4 }
    #         return 5
    #     }
    # }
    # let a = a()
    # write(a)
    # """, "1")

    # test_invalid("""
    # func a() -> Int {
    #     if true {
    #         let a = 5
    #         if true { return 1
    #         } else {/*here it's missing*/}
    #     } else {
    #         return 2
    #     }
    #
    #     while true {
    #         if true { return 3
    #         } else { return 4 }
    #         return 5
    #     }
    # }
    # let a = a()
    # write(a)
    # """, error_other_semantic)

    # test_invalid("""
    # func a() -> String {
    #     if true {
    #         return "kentus"
    #     } else {}
    # }
    # let a = a()
    # write(a)
    # """, error_other_semantic)

    test("""
    func a() -> Int {
        return 4
    }
    let b = a()
    write(b)
    """, "4")

    test_invalid("""
    func a() -> Int {
        return 4.5
    }
    let b = a()
    write(b)
    """, error_call)

    test_invalid("""
    func a() -> Int? {
        let a: String? = nil
        return a
    }
    let b = a()
    write(b)
    """, error_call)

    test_invalid("""
    func a() -> Int {
        return nil
    }
    let b = a()
    write(b)
    """, error_call)

    test("""
    func a() -> Int? {
        return 4
    }
    let b = a()
    write(b)
    """, "4")

    test("""
    func a() -> Int? {
        return nil
    }
    let b = a()
    write(b)
    """, "")

    test("""
    func a() -> Double {
        return 4
    }
    let b = a()
    write(b)
    """, hexfloat(4.0))

    test("""
    func a() -> Double? {
        return 4
    }
    let b = a()
    write(b)
    """, hexfloat(4.0))

    test("""
    func a() -> Double? {
        return 4.5
    }
    let b = a()
    write(b)
    """, hexfloat(4.5))

    test("""
    func a() -> Double? {
        return 4.5
    }
    let b = a()
    write(b)
    """, hexfloat(4.5))

    test_invalid("""
    return
    """, error_other_semantic)

    test_invalid("""
    return 4
    """, error_other_semantic)

    # test_invalid("""
    # if true {
    #     return 4
    # } else {}
    # """, error_other_semantic)

    # expression evaluation

    error_type = ErrorType.error_type

    """
    Standardní binární operátory +, -, * značí sčítání, odčítání a násobení. Jsou-li oba ope-
    randy typu Int, je i výsledek typu Int. Jsou-li oba operandy typu Double, výsledek je
    typu Double. Je-li jeden operand typu Int a druhý Double a celočíselný operand je zá-
    roveň literál, dojde k implicitní konverzi literálu na typ Double. Operátor / značí dělení
    (desetinné pro desetinné operandy, celočíselné pro celočíselné operandy).
    """
    # also, type casting works everywhere
    # https://moodle.vut.cz/mod/forum/discuss.php?d=3207#p5079

    test_expr_valid("4 + 4")
    test_expr_valid("4 - 4.5")
    test_expr_valid("4.5 * 4")
    test_expr_valid("4.5 * 4.5")
    test_expr("7 / 4", "1")  # integer division
    test_expr_valid("4.0 / 4.0")
    test_expr_valid("4 / 4.5")  # everywhere means even here
    test_expr_valid("4.5 / 4")

    test("""
    let a = 5
    let b = a - nil
    """, "", expected_return_code=error_type)

    test("""
    let a: Int? = 5
    let b: Int? = 2
    let c = a - b
    """, "", expected_return_code=error_type)

    test("""
    let a: Int? = 5
    let b = a - 7
    """, "", expected_return_code=error_type)

    """
    Řetězcový operátor + provádí se dvěma operandy typu String jejich konkatenaci.
    Je-li hodnota levého operandu binárního operátoru ?? různá od nil, je výsledkem
    tato hodnota, jinak je výsledkem hodnota pravého operandu, jehož typ musí odpovídat typu
    levého operandu bez zahrnutí nil. Postfixový unární operátor ! slouží pro úpravu typu
    operandu tak, že na zodpovědnost programátora ve výsledku nemůže být nil (jinak může
    dojít k běhové chybě).
    """
    # non-nullable types in lhs of ?? and in ! are not tested
    # https://moodle.vut.cz/mod/forum/discuss.php?d=3250#p5129

    test_expr_invalid("4.5 / \"abc\"")
    test_expr_invalid(" \"abc\" - \"def\" ")
    test_expr_invalid("\"abc\" * 7")
    test_expr(" \"abc\" + \"def\" ", "abcdef")

    test_expr("nil ?? 4", "4")
    test_expr("nil ?? \"abc\" ", "abc")

    test("""
    let a: Int? = 5
    let b = a ?? nil
    """, "", expected_return_code=error_type)

    test("""
    let a: String?
    let b = 4
    let c = a ?? b
    """, "", expected_return_code=error_type)

    test("""
    let a: String?
    let b = "abc"
    let c = b ?? a
    """, "", expected_return_code=error_type)

    test("""
    let a: Int? = 4
    write(a)
    """, "4")

    test("""
    let a: Int? // nil initialization
    let b = a ?? 4
    write(b)
    """, "4")

    test("""
    let a: Int? = 5
    let b = a ?? 4
    write(b)
    """, "5")

    test("""
    let a: Double? = 5
    let b = a ?? 4 // does type casting work?
    """, "")

    test("""
    let a: Int? = 4
    let b = 4 + a! + 4
    write(b)
    """, "12")

    test("""
    let a: Int?
    let b = 4 + a! + 4
    """, "", expect_runtime_error=true)

    test("""
    let a: String?
    let b = 4 + a! + 4
    """, "", expected_return_code=error_type)

    test("""
    let a: String?
    let b = "b" + a!
    write(b)
    """, "", expect_runtime_error=true)

    test_expr_invalid("nil!")

    """
    Pro operátor == platí: Pokud je první operand jiného typu než druhý operand, dochází
    k chybě 7. Číselné literály jsou při sémantických kontrolách implicitně přetypovány na po-
    třebný číselný typ (tj. z Int na Double), avšak u proměnných k implicitním konverzím
    nedochází. Pokud jsou operandy stejného typu, tak se porovnají hodnoty daných operandů.
    Operátor != je negací operátoru ==
    """


    test(f"""
        let result = nil == nil
        write(result)
    ""","",expected_return_code=ErrorType.error_type_inference)
    # test_expr("nil == nil", "true")  # i guess
    test_expr("4 == 4.5", "false")
    test_expr("4.5 != 4", "true")
    # test_expr("nil == 4", "false")
    test_expr(""" "abc" == "def" """, "false")
    test_expr(""" "abc" == "abc" """, "true")
    test_expr_invalid(" \"abc\" != 5")
    test_expr_invalid(" \"abc\" != 5.5")
    # test_expr(" \"abc\" != nil", "true")
    # test_expr(""" "abc" != nil """, "true")

    # test("""
    # let a: Double? = nil
    # let b = nil == a
    # write(b)
    # """, "true")

    test("""
    let a = 5.5
    let b: Double? = 5.5
    let c = a == b
    write(c)
    """, "true")

    test("""
    let a = 5
    let b = 5.5
    let c = a == b
    """, "", expected_return_code=error_type)

    # test_expr("""
    # "abc" != nil
    # """, "true")

    """
    Pro relační operátory <, >, <=, >= platí: Sémantika operátorů odpovídá jazyku Swift.
    Nelze při porovnání mít jeden z operandů jiného typu nebo druhý (např. jeden celé a druhý
    desetinné číslo), ani potenciálně obsahující nil (tj. případný výraz je nejprve potřeba pře-
    vést na výraz typu bez hodnoty nil). U řetězců se porovnání provádí lexikograficky.
    """

    test_expr_invalid("nil >= nil")  # i guess
    test_expr("4 < 4.5", "true")
    test_expr("4.5 > 4", "true")
    test_expr_invalid("nil <= 4")
    test_expr_invalid(""" "abc" <= 4 """)
    test_expr_invalid(""" "abc" <= nil """)
    test_expr(""" "abc" < "def" """, "true")
    test_expr(""" "abc" >= "def" """, "false")
    test_expr(""" "abc" >= "abc" """, "true")
    test_expr(""" "abc" <= "abc" """, "true")
    test_expr_invalid("nil <= 4")

    test("""
    let a: Double? = nil
    let b = 3.2
    let c = a > b
    """, "", expected_return_code=error_type)

    test("""
    let a: Double? = 4
    let b = 3.2
    let c = a > b
    """, "", expected_return_code=error_type)

    # builtin functions

    """
    Příkazy pro načítání hodnot:
    func readString() -> String?
    func readInt() -> Int?
    func readDouble() -> Double?

    Vestavěné funkce ze standardního vstupu načtou jeden řádek ukončený odřádkováním
    nebo koncem souboru (EOF). Funkce readString tento řetězec vrátí bez symbolu
    konce řádku (načítaný řetězec nepodporuje escape sekvence). V případě readInt a
    readDouble jsou jakékoli nevhodné (včetně okolních bílých) znaky známkou špat-
    ného formátu, což vede na návratovou hodnotu nil. Funkce readInt načítá a vrací
    celé číslo, readDouble desetinné číslo. V případě chybějící hodnoty na vstupu (např.
    načtení EOF) nebo jejího špatného formátu je vrácena hodnota nil.
    """

    test("""
    let a = readInt()
    write(a)
    """, "234", program_stdin="234\n")

    test("""
    let a = readInt()
    let b = readInt()
    write(a, b)
    """, "2345", program_stdin="234\n5\n\nabc\n")

    test("""
    let a = readInt()
    write(a)
    """, "234", program_stdin="234")

    test("""
    let a = readInt()
    write(a)
    """, "", program_stdin="")

    test("""
    let a = readInt()
    write(a)
    """, "", program_stdin="abc\n123")

    test("""
    let a = readInt()
    write(a)
    """, "", program_stdin=" 234")

    test("""
    let a = readInt()
    write(a)
    """, "", program_stdin=" 234")

    test("""
    let a = readDouble()
    write(a)
    """, hexfloat(2.5), program_stdin="2.5")

    test("""
    let a = readDouble()
    let b = readDouble()
    write(a, b)
    """, hexfloat(2.5) + hexfloat(4.5), program_stdin="2.5\n4.5")

    # bug in interpreter

    # test("""
    # let a = readDouble()
    # write(a)
    # """, "", program_stdin=" 2.5\n4.5")

    test("""
    let a = readString()
    write(a)
    """, "abc", program_stdin="abc\n")

    test("""
    let a = readString()
    write(a)
    """, "", program_stdin="")

    test("""
    let a = readString()
    let b = readString()
    let c = a! + b!
    write(c)
    """, "abcd", program_stdin="abc\nd\n")

    """
    Příkaz pro výpis hodnot:
    func write ( term1 , term2 , …, term𝑛 )
    Vestavěný příkaz má libovolný počet parametrů tvořených termy oddělenými čárkou.
    Sémantika příkazu je následující: Postupně zleva doprava prochází termy (podrobněji
    popsány v sekci 3.1) a vypisuje jejich hodnoty na standardní výstup ihned za sebe bez
    žádných oddělovačů dle typu v patřičném formátu. Za posledním termem se též nic
    nevypisuje! Hodnota termu typu Int bude vytištěna pomocí '%d'12, hodnota termu
    typu Double pak pomocí '%a'13. Hodnota nil je tištěna jako prázdný řetězec.
    Funkce write nemá návratovou hodnotu.
    """
    # test_expr uses `write`

    test_expr("\"abc\\ndef\"", "abc\ndef")
    test_expr("\"abc#def\"", "abc#def")
    test_expr("\"abc\\\\def\"", "abc\\def")
    # test_expr("true", "true")
    # test_expr("false", "false")
    test_expr("0 - 5", "-5")
    test_expr(" \"\\u{1}\\u{37}\\u{71}\\u{7e}\\u{7f}\"",
              chr(0x1) + chr(0x37) + chr(0x71) + chr(0x7e) + chr(0x7f))
    # test("""
    # let a = 45
    # let b = 2.3
    # let c: Bool? = nil
    # let d = "\\n"
    # write(a, " kentus ", a, d, b, c, " ", d )
    # """, f"45 kentus 45\n{2.3.hex()} \n")

    test("""
    let a = 4 - 8
    let b = Int2Double(a)
    write(b)
    """, hexfloat(-4.0))

    test("""
    let a = 4 - 8.0
    let b = Double2Int(a)
    write(b)
    """, "-4")

    """
    func length(_ 𝑠 : String) -> Int – Vrátí délku (počet znaků) řetězce 𝑠. Např.
    length("x\nz") vrací 3.
    """

    test_expr("length(\"\")", "0")
    test_expr("length(\"a\")", "1")
    test_expr("length(\"\\u{123}\")", "1")
    test_expr("length(\"abc\")", "3")
    test_expr("length(\"a\\nc\")", "3")
    test_expr("length(\"Aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\")", "50")

    """
    func substring(of 𝑠 : String, startingAt 𝑖 : Int, endingBefore 𝑗 : Int) ->
    String? – Vrátí podřetězec zadaného řetězce 𝑠. Druhým parametrem 𝑖 je dán index
    začátku požadovaného podřetězce (počítáno od nuly) a třetím parametrem 𝑗 určuje
    index za posledním znakem podřetězce (též počítáno od nuly).
    Funkce dále vrací hodnotu nil, nastane-li některý z těchto případů:
    • 𝑖 < 0
    • 𝑗 < 0
    • 𝑖 > 𝑗
    • 𝑖 ≥ length(𝑠)
    • 𝑗 > length(𝑠)
    """

    test_expr(
        """ substring(of: "123456789", startingAt: 0, endingBefore: 9) """, "123456789")
    test_expr(
        """ substring(of: "123456789", startingAt: 0, endingBefore: 8) """, "12345678")
    test_expr(
        """ substring(of: "123456789", startingAt: 1, endingBefore: 9) """, "23456789")
    test_expr(
        """ substring(of: "123456789", startingAt: 1, endingBefore: 8) """, "2345678")
    test_expr(
        """ substring(of: "123456789", startingAt: 0, endingBefore: 10) """, "")
    test_expr(
        """ substring(of: "123456789", startingAt: 9, endingBefore: 9) """, "")
    test_expr(
        """ substring(of: "123456789", startingAt: 4, endingBefore: 4) """, "")
    test_expr(
        """ substring(of: "123456789", startingAt: 5, endingBefore: 4) """, "")
    test_expr(
        """ substring(of: "123456789", startingAt: 4, endingBefore: 5) """, "5")
    test("""
    let i = 0 - 4
    let a = substring(of: "123456789", startingAt: i, endingBefore: 4)
    write(a)
    """, "")
    test("""
    let j = 0 - 4
    let a = substring(of: "123456789", startingAt: 0, endingBefore: j)
    write(a)
    """, "")

    # missing argument label in builtin function
    test(
        """ substring("123456789", startingAt: 4, endingBefore: 4) """, "",
        expected_return_code=ErrorType.error_other_semantic)

    """
    func ord(_ 𝑐 : String) -> Int – Vrátí ordinální hodnotu (ASCII) prvního znaku
    v řetězci 𝑐. Je-li řetězec prázdný, vrací funkce 0.

    func chr(_ 𝑖 : Int) -> String – Vrátí jednoznakový řetězec se znakem, jehož
    ASCII kód je zadán parametrem 𝑖. Hodnotu 𝑖 mimo interval [0; 255] řeší odpovídající
    instrukce IFJcode23.
    """

    test_expr("ord(\"\")", "0")
    test_expr("ord(\"a\")", ord("a"))
    test_expr("ord(\"\\u{bA}\")", 0xba)
    test_expr("ord(\"#\")", ord("#"))
    test_expr("ord(\" \")", ord(" "))
    test_expr("ord(\"\\n\")", ord("\n"))

    test_expr("chr(0)", chr(0))
    test_expr("chr(1)", chr(1))
    test_expr("chr(10)", chr(10))
    test_expr("chr(42)", chr(42))
    test_expr("chr(127)", chr(127))

    # test of missing / extra / wrong types of arguments in builtin funtions

    test("chr(\"\")", "", expected_return_code=ErrorType.error_call)
    test("ord(3)", "", expected_return_code=ErrorType.error_call)
    test("ord(3, 4)", "", expected_return_code=ErrorType.error_call)
    test("chr()", "", expected_return_code=ErrorType.error_call)
    test("""
    substring(of: "123456789", startingAt: 0.0)
    """, "", expected_return_code=ErrorType.error_call)
    test("""
    substring(of: "123456789", startingAt: 0)
    """, "", expected_return_code=ErrorType.error_call)
    test("chr(\"\")", "", expected_return_code=ErrorType.error_call)
    test("length(42)", "", expected_return_code=ErrorType.error_call)


if __name__ == "__main__":
    test_all()
    print_exit()
