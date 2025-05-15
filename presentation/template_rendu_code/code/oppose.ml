let oppose (l:int list) : int list =
	let rec aux (ll:int list) (lres:int list) : int list =
		match ll with
		| [] -> lres
		| a::q -> aux q ((-a)::lres)
	in List.rev (aux l [])
	
let test_oppose : unit =
	assert(oppose [] = []);
	assert(oppose [-2; 3; 4; -5] = [2; -3; -4; 5]);
	assert(oppose [10; -10; 10] = [-10; 10; -10])


let intervertit (l: ('a*'b) list) : ('b*'a) list =
	let rec aux (ll:('a*'b) list) (lres:('b*'a) list) : ('b*'a) list  =
		match ll with
		| [] -> lres
		| (x,y)::q -> aux q ((y,x)::lres)
	in List.rev (aux l [])
	
let test_intervertit : unit =
	assert (intervertit [] = []);
	assert (intervertit [(1,2); (3,4)] = [(2,1); (4,3)]);
	assert (intervertit [('a',1); ('b',3); ('z',18)] = [(1,'a'); (3,'b'); (18,'z')] )
	
	
let demultiplie (l: ('a*int) list) : 'a list =
	let rec aux (ll:('a*int) list) (lres:'a list) : 'a list  =
		match ll with
		| [] -> lres
		| (elem,k)::q -> if k<=0 then aux q lres else aux ((elem,k-1)::q) (elem::lres)
	in List.rev (aux l [])
	
	
let test_demultiplie : unit = 
	assert (demultiplie [] = []);
	assert (demultiplie [('a',0)] = []);
	assert (demultiplie [(true,3)] = [true; true; true]);
	assert (demultiplie [('a',1); ('c',3); ('a',2)] = ['a'; 'c'; 'c'; 'c'; 'a'; 'a'])
	 
	
let compresse (l:'a list) : ('a*int) list =
	(*hyp : l <> [] *)
	let rec aux (ll:'a list) (lres:('a*int) list) (elem: 'a) (nb:int) : ('a*int) list  =
		match ll with
		| [] -> (elem,nb)::lres
		| a::q -> if a=elem then aux q lres elem (nb+1) else aux q ((elem,nb)::lres) a 1
	in match l with 
	| [] -> failwith "on ne compresse pas la liste vide"
	| e1::l1 -> List.rev (aux l1 [] e1 1)
	
let test_compresse : unit = 
	assert (compresse [true; true; true] = [(true,3)] );
	assert (compresse [1; 1; 2; 2; 2; 3; 3] = [(1,2); (2,3); (3,2)] );
	assert (compresse ['a'; 'c'; 'c'; 'c'; 'a'; 'a'] = [('a',1); ('c',3); ('a',2)] )



let moyenne (l:int list) : float =
	(*hyp : l <> [] *)
	let rec aux (ll:int list) (somme:int) (cpt:int) : int*int  =
		match ll with
		| [] -> (somme, cpt)
		| a::q -> aux q (somme+a) (cpt+1)
	in let s,c = aux l 0 0
	in float_of_int(s)/.float_of_int(c)
	
	
let test_moyenne : unit =
	assert (moyenne[1; 1; 1; 1; 1] = 1.);
	assert (moyenne[1; 2; 3] = 2.);
	assert (moyenne[-10; 10; 5] = (5./.3.))
	
let moyennes_listes (l: int list list) : float list = 
	(*hyp : les éléments de l sont non vides *)
	let rec aux (ll: int list list) (lres:float list) : float list  =
		match ll with
		| [] -> lres
		| a::q -> aux q ((moyenne a)::lres)
	in List.rev (aux l [])
	
let test_moyennes_listes : unit =
	assert (moyennes_listes[] = []);
	assert (moyennes_listes [[1; 1; 1; 1; 1]; [2;3]] = [1.; 2.5]);
	assert (moyennes_listes [[1; 2; 3]; [3; 2; 1]; [2; 1; 3]] = [2.; 2.; 2.]);
	assert (moyennes_listes[[-10; 10; 5]] = [5./.3.])


let moyenne_bis (l:int list) : float option =
	match l with
	| [] -> None
	| _ -> 
		let rec aux (ll:int list) (somme:int) (cpt:int) : int*int  =
			match ll with
			| [] -> (somme, cpt)
			| a::q -> aux q (somme+a) (cpt+1)
		in let s,c = aux l 0 0
		in Some (float_of_int(s)/.float_of_int(c))


let moyennes_listes_bis (l: int list list) : float list = 
	(*hyp : les éléments de l sont non vides *)
	let rec aux (ll: int list list) (lres:float list) : float list  =
		match ll with
		| [] -> lres
		| a::q -> let m = (moyenne_bis a) in 
			match m with 
			|	None -> aux q lres
			| Some moy -> aux q (moy::lres)
	in List.rev (aux l [])	


let test_moyennes_listes_bis : unit =
	assert (moyennes_listes_bis[] = []);
	assert (moyennes_listes_bis [[1; 1; 1; 1; 1]; [];  [2;3]] = [1.; 2.5]);
	assert (moyennes_listes_bis [[1; 2; 3]; [3; 2; 1]; [2; 1; 3]; [];[]] = [2.; 2.; 2.]);
	assert (moyennes_listes_bis[[-10; 10; 5]; [0]; []] = [5./.3.; 0.])	

let moyennes_listes_ter (l: int list list) : float list =
	List.filter_map moyenne_bis l
	
let test_moyennes_listes_ter : unit =	
	assert (moyennes_listes_ter[] = []);
	assert (moyennes_listes_ter [[1; 1; 1; 1; 1]; [];  [2;3]] = [1.; 2.5]);
	assert (moyennes_listes_ter [[1; 2; 3]; [3; 2; 1]; [2; 1; 3]; [];[]] = [2.; 2.; 2.]);
	assert (moyennes_listes_ter[[-10; 10; 5]; [0]; []] = [5./.3.; 0.])	
