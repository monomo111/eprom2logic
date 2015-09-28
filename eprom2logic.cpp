#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;

string q[1];
string i[1];
unsigned int count_i;
unsigned int count_q;

/*---------------------------------------------------------------------------
 Konvertiert eine Zeichenkette zu einer Zahl
 keine Plasibilitätsprüfung
---------------------------------------------------------------------------*/
unsigned int string_to_int(string input) {
	// Variabele
	unsigned int ret_value;
	
	stringstream strstream (stringstream::in | stringstream::out);
	strstream << input;
	strstream >> ret_value;
	return ret_value;
}

/*---------------------------------------------------------------------------
 Konvertiert ein Zahl zu einer Zeichenkette
 keine Plasibilitätsprüfung
---------------------------------------------------------------------------*/
string int_to_string(unsigned int input) {
	// Variabele
	string ret_value;
	
	stringstream strstream (stringstream::in | stringstream::out);
	strstream << input;
	strstream >> ret_value;
	return ret_value;
}


string remove_white_spaces(char buffer[1024]) {
	string tmp = "";
	//char
	for(unsigned int i = 0; i < 1024; i++) {
		switch(buffer[i]) {
			case ' ':
			case '	':
			break;
			case 0:
			case '#':
				return tmp;
			default:
				tmp += buffer[i];
				break;
		
		}
	}
	return tmp;
}

string load_inputfile(fstream &f){
	string tmp = "";
	char buffer[1024];
	while(f.eof() != true) {
		f.getline(buffer,1024);
		tmp += remove_white_spaces(buffer);
	} 
	return tmp;
}

bool set_io(string input) {
	size_t beginn = input.find("def{");
	if(beginn == string::npos) return false;
	beginn = beginn + 4;
	size_t ende = input.find_first_of('}');
	string tmp = input.substr(beginn, ende - beginn);
	beginn = input.find("i=");
	if(beginn == string::npos) return false;
	beginn = beginn + 2;
	ende = input.find_first_of(';',beginn);
	count_i = string_to_int(input.substr(beginn, ende - beginn));
	beginn = input.find("q=");
	if(beginn == string::npos) return false;
	beginn = beginn + 2;
	ende = input.find_first_of(';',beginn);
	count_q = string_to_int(input.substr(beginn, ende - beginn));
	return true;
}

string get_q_by_id(string input_string, unsigned int i) {
	string search = "q";
	search += int_to_string(i);
	search += '{';
	size_t beginn = input_string.find(search);
	if(beginn == string::npos) return "";
	beginn = beginn + search.size();
	size_t ende = input_string.find_first_of('}',beginn);
	return input_string.substr(beginn, ende - beginn);
} 

string get_input(string in,bool * table) {
	string tmp = "";
	string pos_val;
	unsigned int pos;
	bool make_input = false;
	for(unsigned int i = 0; i < (in.size() );i++) {
		if(make_input == true) {
			switch(in[i]){
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					pos_val += in[i];
					break;
				default:
					make_input = false;
					pos = string_to_int(pos_val);
					pos_val = "";
					if(table[pos] == true){
						tmp += 't';
					}
					else {
						tmp += 'f';
					}
					tmp += in[i];
					break;
			}
		}
		else {
			if((in[i] == 'i') || (in[i] == 'I')) {
				make_input = true;
			}
			else tmp += in[i];
		}
	}
	if(make_input == true) {
		pos = string_to_int(pos_val);
		if(table[pos] == true){
			tmp += 't';
		}
		else {
			tmp += 'f';
		}
	}	
	return tmp;
}

string make_not(string in) {
	string tmp = "";
	for(unsigned int i = 0; i < (in.size() );i++) {
		if((in[i] == 'n') || (in[i] == 'N')) {
			switch(in[i+1]) {
				case 't':
					tmp += 'f';
					i++;
					break;
				case 'f':
					tmp += 't';
					i++;
					break;
				default:
					//error
					break;
			}
		}
		else tmp += in[i];
	}
	return tmp;
}

bool parse(string in, bool * table) {
	size_t start = in.find("(");
	if(start != string::npos) {
		string tmp[1000];
		unsigned int index = 0;
		tmp[0] = "";
		for(unsigned int i = 0 ; i < in.size();i++) {
			switch(in[i]) {
				case '(':
					index = index + 1;
					tmp[index] = "";
					break;
				case ')':
					if(parse(tmp[index],table) == true) {
						index--;
						tmp[index] += "t";
					}
					else {
						index--;
						tmp[index] += "f";
					}
					break;
				default:
					tmp[index] += in[i];
					break;
			}
		}
		if(index > 0) cout << "Parsing error";
		in = tmp[0];
	}
	in = get_input(in, table);
	in = make_not(in);
	string tmp = "";
	while(in.size() > 1) {
		switch(in[0]) {
			case 't':
				switch(in[1]) {
					case 'a':
					case 'A':
						switch(in[2]) {
							case 't':
								tmp = "t";
								tmp += in.substr(3);
								break;
							case 'f':
								tmp = "f";
								tmp += in.substr(3);
								break;
							default:
								//error
								break;
						}
						break;
					case 'o':
					case 'O':
						tmp = "t";
						tmp += in.substr(3);
						break;
					case 'x':
					case 'X':
						switch(in[2]) {
							case 't':
								tmp = "f";
								tmp += in.substr(3);
								break;
							case 'f':
								tmp = "t";
								tmp += in.substr(3);
								break;
							default:
								//error
								break;
						}
						break;
					default:
						cout <<  "error";
						break ;
				}
				break;
			case 'f':
				switch(in[1]) {
					case 'a':
					case 'A':
						tmp = "f";
						tmp += in.substr(3);
						break;
					case 'o':
					case 'O':
						switch(in[2]) {
							case 't':
								tmp = "t";
								tmp += in.substr(3);
								break;
							case 'f':
								tmp = "f";
								tmp += in.substr(3);
								break;
							default:
								//error
								break;
						}
						break;
					case 'x':
					case 'X':
						switch(in[2]) {
							case 't':
								tmp = "t";
								tmp += in.substr(3);
								break;
							case 'f':
								tmp = "f";
								tmp += in.substr(3);
								break;
							default:
								//error
								break;
						}
						break;
					default:
						// error
						break;
				}
				break;	
			default:
				break;
		}
		in = tmp;
	}
	//cout << "erg "<< in << endl;
	if(in[0] == 't')return true;
	return false;
}


int main( int a, char **b) {
	// Variabelen
	fstream f;
	fstream e;
	string inputfile = "";
	inputfile = b[1];
	string input_string;
	string  * q_string;
	bool * t_table;
	// Es muss ein Paramter angegeben werden
	if(a == 2) {
		f.open(inputfile.c_str());
		input_string = load_inputfile(f);
		f.close();
		set_io(input_string);
		cout << input_string << endl;
		unsigned long size_of_bytes = pow (2,count_i);
		cout << size_of_bytes << " " << count_i << endl;
		q_string = new string[count_q];
		t_table = new bool[count_i];
		// loading Q-Strings
		for(unsigned int i = 0; i < count_q; i++) {
			q_string[i] = get_q_by_id(input_string, i);
			cout << q_string[i] << endl;
			
		}
		// open outputfile
		inputfile += ".bin";
		e.open(inputfile.c_str());
		unsigned char out = 0;
		for(unsigned long i = 0; i < size_of_bytes; i++) {
			cout << "I:" ;
			for(unsigned int t = 0; t < count_i; t ++) {
				unsigned long med = pow(2,t); 
				if((i & med) > 0){
					cout << "1";
					t_table[t] = true;
				}
				else {
					cout << "0";
					t_table[t] = false;
				}
			}
			cout << " Q";
			for(unsigned int c = 0 ; c < count_q; c++) {
				if(parse(q_string[c],t_table)) {
					out = out + pow(2,c);
					cout << "1";
					//e << "1";
				}
				else {
					cout << "0";
					//e << "0";
				}
			}
			e << out;
			cout << endl;
			out = 0;
		}
		e.close();
		
	}
	else {
		cout << "Bitte Datei angeben." << endl;
	}
	return 0;
}
