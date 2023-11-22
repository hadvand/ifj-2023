/** 
 * ifj23.swift - Soubor pro zlepseni kompatibility Swift 5 a IFJ23
 * 
 * Skript pomaha preklenout zakladni rozdily v obou jazycich pro ucely testovani, 
 * avsak zdaleka neodstranuje vsechny odlisnosti! 
 * Pouziti (upravte v nasledujicim prikazu nazev soubor s vasim programem na serveru Merlin): 
 * 		swift <(cat vas_program.swift ifj23.swift)
 * 
 * Swift version 5.8
 * 
 * @author Zbynek Krivka <krivka@fit.vut.cz>
 * @author Radim Kocman <kocman@fit.vut.cz>
 * 
 */
 
import Foundation

// Embedded functions of IFJ23

func write(_ args : Any?...)  // anonymous parameter name/label
{
  for arg in args 
  {
    if let arg // non-nil values 
    {
      if type(of: arg) == Double.self  // floating-point value prints in hexa format in IFJ23
      {
        let a : Double = arg as! Double
        print(String(format: "%a", a), separator:"", terminator:"")
      }  
      else
      {
        print(arg, separator:"", terminator:"")
      }
    }
    else  // nil value
    {
      print("nil", separator:"", terminator:"")
    }  
  }
}

func readString() -> String?
{
  return readLine()  // nil for EOF
}

func readInt() -> Int?
{
  if let input = readLine()
  {
    return Int(input)    // any unexpected character including whitespace means the number is invalid and returns nil
  }
  return nil   // nil for EOF
}

func readDouble() -> Double?
{
  if let input = readLine()
  {
    return Double(input)   // any unexpected character including whitespace means the number is invalid and returns nil
  }
  return nil   // nil for EOF  
}

func Int2Double(_ int : Int) -> Double
{
  return Double(int)
}

func Double2Int(_ dbl : Double) -> Int
{
  return Int(dbl)
}

func length(_ str: String) -> Int
{
  return str.count
}

func ord(_ str : String) -> Int
{
  if str.count >= 1
  {
    return Int(Character(extendedGraphemeClusterLiteral: str[str.startIndex]).asciiValue ?? 255)
  }
  return 0
}

func chr(_ i : Int) -> String
{ 
  return String(UnicodeScalar(i) ?? UnicodeScalar(0)) // out of 0..255 it throws an error
}

func substring(of s: String, startingAt i: Int, endingBefore j: Int) -> String?
{
  if (i < 0 || j < 0 || i > j || i >= s.count || j > s.count) {
    return nil
  } 
  let fromIdx = s.index(s.startIndex, offsetBy: i)
  let toIdx = s.index(s.startIndex, offsetBy: j)
  let substring = s[fromIdx..<toIdx] 
  return String(substring)
}

