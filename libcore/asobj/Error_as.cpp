// Error_as.cpp:  ActionScript "Error" class, for Gnash.
//
//   Copyright (C) 2005, 2006, 2007, 2008, 2009 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifdef HAVE_CONFIG_H
#include "gnashconfig.h"
#endif

#include "Error_as.h"
#include "as_object.h" // for inheritance
#include "log.h"
#include "fn_call.h"
#include "smart_ptr.h" // for boost intrusive_ptr
#include "builtin_function.h" // need builtin_function
#include "GnashException.h" // for ActionException
#include "Object.h" // for AS inheritance
#include "VM.h" // for addStatics

#include <sstream>

namespace gnash {

namespace {
    as_value error_toString(const fn_call& fn);
    as_value error_ctor(const fn_call& fn);

    as_object* getErrorInterface();
    void attachErrorInterface(as_object& o);
}


class Error_as: public as_object
{

public:

	Error_as()
		:
		as_object(getErrorInterface())
	{}

};


// extern 
void Error_class_init(as_object& where)
{
	// This is going to be the Error "class"/"function"
	// in the 'where' package
	boost::intrusive_ptr<builtin_function> cl;
	cl = new builtin_function(&error_ctor, getErrorInterface());

	// Register _global.Error
	where.init_member("Error", cl.get());
}


namespace {

as_object*
getErrorInterface()
{
	static boost::intrusive_ptr<as_object> o;

	if ( ! o )
	{
		o = new as_object(getObjectInterface());
		VM::get().addStatic(o.get());

		attachErrorInterface(*o);

	}

	return o.get();
}


void
attachErrorInterface(as_object& o)
{
    int flags = 0;
    o.init_member("toString", new builtin_function(error_toString), flags);
    o.init_member("message", "Error", flags);
    o.init_member("name", "Error", flags);
}


as_value
error_toString(const fn_call& fn)
{
 	boost::intrusive_ptr<Error_as> ptr = ensureType<Error_as>(fn.this_ptr);

    string_table& st = getStringTable(*ptr);
    as_value message;
    ptr->get_member(st.find("message"), &message);

	return as_value(message);   
}

/// "e = new Error();" returns an Error, "e = Error"; returns undefined.
as_value
error_ctor(const fn_call& fn)
{

    if (!fn.isInstantiation()) return as_value();

	boost::intrusive_ptr<Error_as> err = new Error_as;
	
    string_table& st = getStringTable(fn);
    if (fn.nargs > 0)
	{
		err->set_member(st.find("message"), fn.arg(0));
	}

	return as_value(err.get()); // will keep alive
}

}
} // end of gnash namespace
