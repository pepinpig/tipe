type nb =
  | Int of int
  | Flo of float

let add (x:nb)(y:nb):nb =
  match x,y with
  | Int a, Int b -> Int(a + b)
  | Int a, Flo b -> Flo((float_of_int a) +. b)
  | Flo a, Int b -> Flo( a +. (float_of_int b))
  | Flo a, Flo b -> Flo(a +. b)
