#include "lesh.hpp"

Value Env::lookup_variable_value(Value var){
	list<Frame>::iterator	i;
	for( i=frames.begin(); i!=frames.end(); i++ ){ // list
		Frame	f = *i;
		Frame::iterator	found = f.find(var.sym);//map
		if( found != f.end() ){
			return found->second;
		}
	}
	cout << "UNBIND ERROR" << endl;
	return Value("ooERRORoo");
}

Value Env::define_variable(string var,Value val){
	list<Frame>::iterator	i;
	for ( i=frames.begin(); i!=frames.end(); i++ ){ // list
		Frame::iterator	found = i->find(var);//map
		if ( found != i->end() ){ // found !!
			i->erase( found );
			break;
		}
	}

	frames.begin()->insert( make_pair(var,val));
	return Value("ok");
}

void Env::extend_environment(Vlist syms, Vlist vals){
	frames.push_front( make_frame( syms, vals ) );
}
