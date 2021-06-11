/* $j20 */
#include "semantics.h"

using namespace std;

void tSM::init() {
	globals.clear();
	locals.clear();
	params.clear();
	scope = 0; // вне процедуры

// константы:
	globals["e"] = tgName(VAR|DEFINED|BUILT);
	globals["pi"] = tgName(VAR|DEFINED|BUILT);

// элементарные процедуры:
	globals["newline"] = tgName(PROC|DEFINED|BUILT,"", 0);

	globals["abs"] = tgName(PROC|DEFINED|BUILT,"", 1);
	globals["sqrt"] = tgName(PROC|DEFINED|BUILT,"", 1);
	globals["log"] = tgName(PROC|DEFINED|BUILT,"", 1);
	globals["exp"] = tgName(PROC|DEFINED|BUILT,"", 1);
	globals["sin"] = tgName(PROC|DEFINED|BUILT,"", 1);
	globals["cos"] = tgName(PROC|DEFINED|BUILT,"", 1);
	globals["tan"] = tgName(PROC|DEFINED|BUILT,"", 1);
	globals["atan"] = tgName(PROC|DEFINED|BUILT,"", 1);
	globals["not"] = tgName(PROC|DEFINED|BUILT,"", 1);
	globals["display"] = tgName(PROC|DEFINED|BUILT,"", 1);

	globals["remainder"] = tgName(PROC|DEFINED|BUILT,"", 2);
	globals["quotient"] = tgName(PROC|DEFINED|BUILT,"", 2);
	globals["expt"] = tgName(PROC|DEFINED|BUILT,"", 2);

	return;
}

int tSM::p01() { // S -> PROG
	bool error = false;
	for (tGlobal::iterator it = globals.begin(); it != globals.end(); ++it) {
	// Просмотреть таблицу глобальных имен и в сообщении об ошибках указать имена
	// ВСЕХ вызванных, но не определенных процедур, а также использованных, но не определенных
	// глобальных переменных.
		string tmp_name = it->first;
		string tmp_line = it->second.line;

		if (!it->second.test(DEFINED) && it->second.test(USED) && !it->second.test(BUILT) && (it->second.test(PROC))) {
			// процедура '...' не определена, но использоана!
			// the procedure '...' is not defined, but used!
			ferror_message += "Error[01-1] in line " + tmp_line + ": procedure '" + tmp_name + "' not defined, but used!\n";
			error = true;
		}
		if (!it->second.test(DEFINED) && it->second.test(USED) && !it->second.test(BUILT) && (it->second.test(VAR))) {
			// глобальная переменная '...' не определена, но использована!
			// the global variable '...' is not defined, but is used!
			ferror_message += "Error[01-2] in line " + tmp_line + ": the global variable '" + tmp_name + "' not defined, but used!\n";
			error = true;
		}
	} //for...
	if (error) return 1;
	return 0;
}

int tSM::p02() { //     PROG -> CALCS
	return 0;}

int tSM::p03() { //     PROG -> DEFS
	return 0;}

int tSM::p04() { //     PROG -> DEFS CALCS
	return 0;}

int tSM::p05() { //        E -> $id
	string tmp_name = S1->name;
	string tmp_line = S1->line;

	switch (scope) {
		case 2: 
			if (locals.count(tmp_name)) break;
		case 1: 
			if (params.count(tmp_name)) break;
		default: 
			tgName& ref = globals[tmp_name];
			if (ref.empty()) {
				ref = tgName(VAR|USED, tmp_line);
				break;
			}
			if (ref.test(VAR)) {
				ref.set(USED);
				break;
			}
			if (ref.test(BUILT)) {
				// встроенную процедуру '...' нельзя использовать в качестве переменной
				// the built-in '...' procedure cannot be used as a variable
				ferror_message += "Error[05-1] in line: "+ tmp_line +": the built-in '"
								+ tmp_name +
								  "' procedure \n\t\t\t cannot be used as a variable!\n";
				return 1;
			}
			// имя '...' нельзя использовать для ссылки на переменную, в строке ... оно ранее объявлено как процедура
			// the name '...' cannot be used to refer to a variable; it was previously declared as a procedure in line ...
			ferror_message += "Error[05-2] in line: "+ tmp_line +": the name '"
							+ tmp_name 
							+ "' cannot be used to refer to a variable;\n\t\t\tit was previously declared as a procedure in line "
							+ ref.line +"!\n";

			return 1;

	} //switch...

	return 0;
}

int tSM::p06() { //        E -> $int
	return 0;}

int tSM::p07() { //        E -> $dec
	return 0;}

int tSM::p08() { //        E -> AREX
	return 0;}

int tSM::p09() { //        E -> COND
	return 0;}

int tSM::p10() { //        E -> CPROC
	string tmp_name = S1->name; 
	int	tmp_count = S1->count;
	string tmp_line = S1->line;
	int tmp_types = S1->types;
	switch (scope) {
		case 2:
			if (locals.count(tmp_name)) {
				// локальная переменная переоределяет процедуру!
				// the local variable overrides the procedure!
				ferror_message += "Error[10-1] in line: "+ tmp_line +": the local variable '" 
								+ tmp_name 
								+ "' overrides the procedure!\n"; 
				return 1;
			}
		case 1: 
			if (params.count(tmp_name)) {
				// параметр переоределяет процедуру!
				// the parameter overrides the procedure!
				ferror_message += "Error[10-2] in line: " + tmp_line 
								+ ": the parameter '" + tmp_name 
								+ "' overrides the procedure!\n";
				return 1;
			}

		default:
			tgName& ref = globals[tmp_name];
			if (ref.empty()) {
				ref = tgName(PROC|USED, tmp_line, tmp_count, tmp_types);
				break;
			}
			if (!ref.test(PROC)) {
				// '...' не процедура!
				// '...' is not a procedure!
				ferror_message += "Error[10-3] in line: "+ tmp_line + ": '" 
								+ tmp_name + "' is not a procedure!\n";
				return 1;
			}
			if (ref.arity != tmp_count) { 
				// процедура ожидает / уже была вызвана с, ... аргументов!
				// the procedure expects / has already been called with, ... arguments!
				ferror_message += "Error[10-4] in line: "+ tmp_line +": '" + tmp_name + "' ";
				if (ref.test(DEFINED)) {
					ferror_message += "expects ";
				} else {
					ferror_message += "has been called already (line: " + ref.line + ")\n\t with ";
				}				
				ferror_message += Uint_to_str(ref.arity) + " argument"
								+ ((ref.arity != 1) ? "s" : "")
								+ ", given: " + Uint_to_str(tmp_count) + " !\n";
			
				return 1;
			}
			ref.set(USED);
	}

	return 0;
}

int tSM::p11() { //     AREX -> HAREX E )
	return 0;}

int tSM::p12() { //    HAREX -> ( AROP
	return 0;}

int tSM::p13() { //    HAREX -> HAREX E
	return 0;}

int tSM::p14() { //     AROP -> +
	return 0;}

int tSM::p15() { //     AROP -> -
	return 0;}

int tSM::p16() { //     AROP -> *
	return 0;}

int tSM::p17() { //     AROP -> /
	return 0;}

int tSM::p18() { //    CPROC -> HCPROC )
	return 0;
}

int tSM::p19() { //   HCPROC -> ( $id
	S1->name = S2->name;
	S1->count = 0;
	return 0;
}

int tSM::p20() { //   HCPROC -> HCPROC E
	++S1->count;
	return 0;}

int tSM::p21() { //     COND -> ( cond BRANCHES )
	return 0;}

int tSM::p22() { // BRANCHES -> CLAUS ELSE
	return 0;}

int tSM::p23() { //    CLAUS -> ( BOOL CLAUSB )
	return 0;}

int tSM::p24() { //   CLAUSB -> E
	return 0;}

int tSM::p25() { //   CLAUSB -> INTER CLAUSB
	return 0;}

int tSM::p26() { //     ELSE -> ( else ELSEB )
	return 0;}

int tSM::p27() { //    ELSEB -> E
	return 0;}

int tSM::p28() { //    ELSEB -> INTER ELSEB
	return 0;}

int tSM::p29() { //      STR -> $str
	return 0;}

int tSM::p30() { //      STR -> SCOND
	return 0;}

int tSM::p31() { //    SCOND -> ( cond SBRANCHES )
	return 0;}

int tSM::p32() { // SBRANCHES -> SELSE
	return 0;}

int tSM::p33() { // SBRANCHES -> SCLAUS SBRANCHES
	return 0;}

int tSM::p34() { //   SCLAUS -> ( BOOL STR )
	return 0;}

int tSM::p35() { //    SELSE -> ( else STR )
	return 0;}

int tSM::p36() { //     BOOL -> $bool
	return 0;}

int tSM::p37() { //     BOOL -> $idq
	string tmp_name = S1->name;
	string tmp_line = S1->line;

	bool in_scope = false;
	if ((scope > 1 && locals.count(tmp_name)) || (scope > 0 && params.count(tmp_name))) in_scope = true;

	if (!in_scope) {
		//булева переменная не определена в данной области видимости!
		// bool variable is not defined in this scope!
		ferror_message += "Error[37-1] in line: "+ tmp_line + ": bool variable '" +
						  tmp_name + "' is not defined in this scope!\n";
		return 1;
	}
	return 0;
}

int tSM::p38() { //     BOOL -> REL
	return 0;}

int tSM::p39() { //     BOOL -> ( not BOOL )
	return 0;
}

int tSM::p40() { //     BOOL -> CPRED
	return 0;}

int tSM::p41() { //    CPRED -> HCPRED )
	string tmp_name = S1->name;
	string tmp_line = S1->line;
 	int tmp_count = S1->count;
	int tmp_types = S1->types;
	switch (scope) {
		case 1: 
			if(params.count(tmp_name)){
				//параметр переопределяет предикат!
				//the parameter overrides the predicate!
      			ferror_message += "Error[41-1] in line: "+ tmp_line + ": '" 
				  				+ tmp_name + "' overrides the predicate!\n";
      			return 1;
    		}

		default:
			tgName& ref = globals[tmp_name];
			if (ref.empty()) {
				ref = tgName(PROC|USED, tmp_line,tmp_count, tmp_types);
				break;
			}
			if(ref.arity != tmp_count) {
				//предикат ожидает/был уже вызван с ... параметрами
				// the predicate expects/was already called with ... parameters
				ferror_message += "Error[41-2] in line: "+ tmp_line + ": '"+ tmp_name + "' ";
				if (ref.test(DEFINED)) {
					ferror_message += "expects ";
				} else {
					ferror_message += "has been called already (line: " + ref.line + ")\n\t with ";
				}	
				ferror_message += Uint_to_str(ref.arity) + " argument"
								+ (ref.arity!=1 ?"s":"")
								+ ", given: " + Uint_to_str(tmp_count) +"!\n";
				return 1;
			}
			if (ref.types != tmp_types) {
				string prev_types = [&ref] () {
					string ans = "(";
					for (int i = 0; i < ref.arity; ++i) {
						if (i != 0) {
							ans += " ";
						}
						ans += ((ref.types & (1 << i)) ? "BOOL" : "E");
					}
					return ans + ")";
				} ();
				string cur_types = [&tmp_count, &tmp_types] () {
					string ans = "(";
					for (int i = 0; i < tmp_count; ++i) {
						if (i != 0) {
							ans += " ";
						}
						ans += ((tmp_types & (1 << i)) ? "BOOL" : "E");
					}
					return ans + ")";
				} ();
				// предикат ожидает/был уже вызван с ... параметрами типа ..., даны ...!
				// the predicate expects/was already called with ... parameters like ..., are given ...!
				ferror_message += "Error[41-3] in line: "+ tmp_line + ": '"+ tmp_name + "' ";
				if (ref.test(DEFINED)) {
					ferror_message += "expects ";
				} else {
					ferror_message += "has been called already (line: " + ref.line + ")\n\t with ";
				}	
				ferror_message += prev_types + " types" + ", are given: " + cur_types +"!\n";
				return 1;
			}
			ref.set(USED);
	}
	return 0;
}

int tSM::p42() { //   HCPRED -> ( $idq
	S1->name = S2->name;
	S1->count = 0;
	return 0;
}

int tSM::p43() { //   HCPRED -> HCPRED ARG
	S1->types = S1->types | (S2->types << S1->count);
	++S1->count;
	return 0;
}

int tSM::p44() { //      ARG -> E
	S1->types = 0;
	return 0;
}

int tSM::p45() { //      ARG -> BOOL
	S1->types = 1;
	return 0;
}

int tSM::p46() { //      REL -> ( = E E )
	return 0;}

int tSM::p47() { //      REL -> ( > E E )
	return 0;}

int tSM::p48() { //      SET -> HSET E )
	return 0;}

int tSM::p49() { //     HSET -> ( set! $id
	string tmp_name = S3->name;
	string tmp_line = S3->line;
	switch (scope) {
		case 2:
			if (locals.count(S3->name)) {
				return 0;
			}
		case 1:
			if (params.count(S3->name)) {
				return 0;
			}
		default:
			tgName& ref = globals[tmp_name];
			if (ref.empty()) {
				ref = tgName(VAR | USED);
				return 0;
			}
			if (ref.test(VAR) && ref.test(BUILT)) {
				//переменная не может быть переопределена!
				// the variable cannot be overridden!
				ferror_message += "Error[49-1] in line: "+ tmp_line + ": variable '" 
								+ S3->name + "' can't be overridden!\n";
				return 1;
			}
			ref.set(USED);
	}
	return 0;
}

int tSM::p50() { //  DISPSET -> ( display E )
	return 0;}

int tSM::p51() { //  DISPSET -> ( display BOOL )
	return 0;}

int tSM::p52() { //  DISPSET -> ( display STR )
	return 0;}

int tSM::p53() { //  DISPSET -> ( newline )
	return 0;}

int tSM::p54() { //  DISPSET -> SET
	return 0;}

int tSM::p55() { //    INTER -> DISPSET
	return 0;}

int tSM::p56() { //    INTER -> E
	return 0;}

int tSM::p57() { //    CALCS -> CALC
	return 0;}

int tSM::p58() { //    CALCS -> CALCS CALC
	return 0;}

int tSM::p59() { //     CALC -> E
	return 0;}

int tSM::p60() { //     CALC -> BOOL
	return 0;}

int tSM::p61() { //     CALC -> STR
	return 0;}

int tSM::p62() { //     CALC -> DISPSET
	return 0;}

int tSM::p63() { //     DEFS -> DEF
	return 0;}

int tSM::p64() { //     DEFS -> DEFS DEF
	return 0;}

int tSM::p65() { //      DEF -> PRED
	return 0;}

int tSM::p66() { //      DEF -> VAR
	return 0;}

int tSM::p67() { //      DEF -> PROC
	return 0;}

int tSM::p68() { //     PRED -> HPRED BOOL )
	string tmp_name = S1->name; 
	int tmp_count = S1->count;
	int tmp_types = S1->types;
	string tmp_line = S1->line;
	tgName& ref = globals[tmp_name];
	if (ref.empty()) {
		params.clear();
		scope = 0;
		ref = tgName(PROC | DEFINED, tmp_line, tmp_count, tmp_types);
		return 0;
	}
	if (ref.test(DEFINED)) {
		// предикат уже определен!
		// the predicate is already defined!
		ferror_message += "Error[68-1] in line: "+ tmp_line + ": predicate '" + tmp_name 
						+ "' is already defiend (line: " + ref.line + ")!\n";
		return 1;
	}
	if (ref.arity != tmp_count) {
		// предикат ожидает/был уже вызван с аргументами ..., даны ...!
		// the predicate expects/has already been called with arguments ..., are given ...!
		ferror_message += "Error[68-2] in line: "+ tmp_line + ": predicate '" + tmp_name + "' ";
		if (ref.test(DEFINED)) {
			ferror_message += "expects";
		}
		else {
			ferror_message += "has been called already (line: " + ref.line + ")\n\t with ";
		}
		ferror_message += Uint_to_str(ref.arity) + " argument" 
						+ (ref.arity != 1 ? "s" : "") + ", are given: " 
						+ Uint_to_str(tmp_count) + "!\n";
		return 1;
	}
	if (ref.types != tmp_types) {
			string prev_types = [&ref] () {
				string ans;
				for (int i = 0; i < ref.arity; ++i) {
					if (i != 0) {
						ans += " ";
					}
					ans += ((ref.types & (1 << i)) ? "BOOL" : "E");
				}
				return ans;
			} ();
			string cur_types = [&tmp_count, &tmp_types] () {
				string ans;
				for (int i = 0; i < tmp_count; ++i) {
					if (i != 0) {
						ans += " ";
					}
					ans += ((tmp_types & (1 << i)) ? "BOOL" : "E");
				}
				return ans;
			} ();
			//предикат был уже вызван с типами ..., даны ...!
			// the predicate was already called with the types ..., given ...!
     		ferror_message += "Error[68-3] in line: "+ tmp_line + ": '"+ tmp_name + "' "
							+ "has been called already (line: " + ref.line + ")\n\t with "
							+ prev_types +" types"
							+ ", given: " + cur_types +"!\n";
     		return 1;
	}
	params.clear();
	scope = 0;
	ref.set(DEFINED);
	return 0;
}

int tSM::p69() { //    HPRED -> PDPAR )
	scope = 1;
	return 0;
}

int tSM::p70() { //    PDPAR -> ( define ( $idq
	S1->name = S4->name;
	S1->count = 0;
	S1->types = 0;
	return 0;
}

int tSM::p71() { //    PDPAR -> PDPAR $idq
	string tmp_name = S2->name;
	S1->types = S1->types | (1 << S1->count);
	++S1->count;
	params.insert(tmp_name);
	return 0;
}

int tSM::p72() { //    PDPAR -> PDPAR $id
	string tmp_name = S1->name;
	string tmp_line = S2->line;
	if (params.count(S2->name)) {
		//предикат дублирует имя параметра!
		// the predicate duplicates the parameter name!
		ferror_message += "Error[72-1] in line: "+ tmp_line + ": predicate '" + tmp_name 
						+ "' duplicates parametr name '" + S2->name + "'!\n";
		return 1;
	}
	++S1->count;
	params.insert(S2->name);
	return 0;
}

int tSM::p73() { //      VAR -> VARDCL E )
	return 0;}

int tSM::p74() { //   VARDCL -> ( define $id
	S1->name = S3->name;
	string tmp_name = S1->name;
	string tmp_line = S3->line;
	tgName& ref = globals[tmp_name];
	if (ref.empty()) {
		ref = tgName(VAR | DEFINED, tmp_line);
		return 0;
	}
	if (ref.test(DEFINED)) {
		//переопределение переменной!
		// redefining a variable!
		ferror_message += "Error[74-1] in line: "+ tmp_line + ": redefining a '" + tmp_name 
						+ "' variable (line: " + ref.line + ")!\n";
		return 1;
	}
	if (ref.test(PROC) && ref.test(USED)) {
		// переопределение уже использованной переменной!
		// redefining an already used variable!
		ferror_message += "Error[74-2] in line: "+ tmp_line + ": redefintion of already used'" + tmp_name 
						+ "' already used as procedure (line: " + ref.line + ")!\n";
			return 1;
	}
	ref.set(DEFINED);

	return 0;
}

int tSM::p75() { //     PROC -> HPROC BLOCK )
	
	string tmp_name = S1->name;
	string tmp_line = S1->line;
	int tmp_count = S1->count;
	int tmp_types = S1->types;

	tgName& ref = globals[tmp_name];
	if (ref.empty()) {
		params.clear();
		scope = 0;
		ref = tgName(PROC | DEFINED, tmp_line , tmp_count, tmp_types);
		return 0;
	}
	ref.set(DEFINED);
	
	params.clear();
	scope = 0;
	return 0;
}

int tSM::p76() { //     PROC -> HPROC E )
	// точка анализа выходит из тела процедуры
	params.clear();
	scope = 0;
	return 0;
}

int tSM::p77() { //    HPROC -> PCPAR )
	int tmp_count = S1->count;
	int tmp_types = S1->types;
	string tmp_name = S1->name;
	string tmp_line = S1->line;

	tgName& ref = globals[tmp_name];
	
	switch (scope) {
		default:
			if (ref.empty()) {
				ref = tgName(PROC|DEFINED, tmp_line, tmp_count, tmp_types);
				break;
			}

			if (ref.test(PROC) == false){
				//процедура использована в качестве глобальной переменной!
				//the procedure is used as a global variable!
				ferror_message += "Error[77-1] in line: "+ tmp_line + ": '" 
								+ tmp_name + "' is used as global variable!\n";
				return 1;
			}

			if (ref.test(DEFINED) == true) {
				//переопределение процедуры!
				// redefining the procedure!
				ferror_message += "Error[77-2] in line: "+ tmp_line + ": redefining the '" 
								+ tmp_name + "' procedure (line: " + ref.line + ")!\n";
				return 1;
			}
			
			if (ref.arity != tmp_count) {
				//процедура ожидает/уже была вызвана с ... параметрами, получено ...!
				//the procedure is waiting for/has already been called with ... parameters, received ...!
				ferror_message += "Error[77-3] in line: "+ tmp_line + ": procedure '"
								+ tmp_name + "' ";
				if (ref.test(DEFINED)) {
					ferror_message += "expects ";
				} else {
					ferror_message += "has been called already (line: " + ref.line + ")\n\t with ";
				}
				ferror_message +=  Uint_to_str(ref.arity) + " argument"
								+ ((ref.arity != 1) ? "s" : "")
								+ ", given: " 
								+ Uint_to_str(tmp_count) +"!\n";
				return 1;
			}
			ref.set(DEFINED);
   	} 
	// точка анализа входит в тело процедуры
	scope = 1;
	return 0;
}

int tSM::p78() { //    HPROC -> HPROC INTER
	return 0;}

int tSM::p79() { //    PCPAR -> ( define ( $id
	S1->name = S4->name;
	S1->count = 0;
	return 0;
}

int tSM::p80() { //    PCPAR -> PCPAR $id
	string tmp_line = S2->line;
	string tmp_name = S2->name;
	if (params.count(S2->name)) {
		// в процедуре '...' дублируется параметр '...'!
		//the '...' procedure duplicates the '...' parameter!
		ferror_message += "Error[80-1] in line: "+ tmp_line +": the procedure '"
						+ S1->name + "' duplicates \n\t\t\tthe '"
						+ tmp_name + "' parameter!\n";
		return 1;
	}

	if (tmp_name == S1->name) {
		// у процедуры '...' такое же имя, как у ее параметра!
		//procedure '...' has the same name as its parameter!
		ferror_message += "Warning[80-2] in line: "+ tmp_line +": procedure '"
						+ S1->name + "'has the same name \n\t\t\tas its parameter!\n";
	}

	params.insert(tmp_name);
	++S1->count;
	return 0;
}

int tSM::p81() { //    BLOCK -> HBLOCK E )
	scope = 1;
	locals.clear();
	return 0;
}

int tSM::p82() { //   HBLOCK -> BLVAR )
	scope = 2;
	return 0;
}

int tSM::p83() { //   HBLOCK -> HBLOCK INTER
	return 0;}

int tSM::p84() { //    BLVAR -> ( let ( LOCDEF
	S1->name = S4->name;
	S1->count = 1;
	locals.insert(S1->name);
	return 0;
}

int tSM::p85() { //    BLVAR -> BLVAR LOCDEF
	++S1->count;
	locals.insert(S2->name);
	return 0;
}

int tSM::p86() { //   LOCDEF -> ( $id E )
	S1->name = S2->name;
	return 0;
}

//_____________________
int tSM::p87(){return 0;} int tSM::p88(){return 0;} 
int tSM::p89(){return 0;} int tSM::p90(){return 0;} 
int tSM::p91(){return 0;} int tSM::p92(){return 0;} 
int tSM::p93(){return 0;} int tSM::p94(){return 0;} 
int tSM::p95(){return 0;} int tSM::p96(){return 0;} 
int tSM::p97(){return 0;} int tSM::p98(){return 0;} 
int tSM::p99(){return 0;} int tSM::p100(){return 0;} 
int tSM::p101(){return 0;} int tSM::p102(){return 0;} 
int tSM::p103(){return 0;} int tSM::p104(){return 0;} 
int tSM::p105(){return 0;} int tSM::p106(){return 0;} 
int tSM::p107(){return 0;} int tSM::p108(){return 0;} 
int tSM::p109(){return 0;} int tSM::p110(){return 0;} 


