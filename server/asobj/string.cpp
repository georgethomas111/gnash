// 
//   Copyright (C) 2005, 2006 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// 
//
//

/* $Id: string.cpp,v 1.7 2006/11/22 09:28:37 strk Exp $ */

// Implementation of ActionScript String class.

#include "tu_config.h"
#include "gstring.h"
#include "smart_ptr.h"
#include "fn_call.h"
#include "as_object.h" 
#include "builtin_function.h" // need builtin_function
#include "log.h"
#include "array.h"

namespace gnash {

// Forward declarations
static void string_concat(const fn_call& fn);
static void string_slice(const fn_call& fn);
static void string_split(const fn_call& fn);
static void string_last_index_of(const fn_call& fn);
static void string_sub_str(const fn_call& fn);
static void string_sub_string(const fn_call& fn);
static void string_index_of(const fn_call& fn);
static void string_from_char_code(const fn_call& fn);
static void string_char_code_at(const fn_call& fn);
static void string_char_at(const fn_call& fn);
static void string_to_upper_case(const fn_call& fn);
static void string_to_lower_case(const fn_call& fn);
static void string_to_string(const fn_call& fn);
static void string_ctor(const fn_call& fn);

static void
attachStringInterface(as_object& o)
{
	// TODO fill in the rest
	o.set_member("concat", &string_concat);
	o.set_member("slice", &string_slice);
	o.set_member("split", &string_split);
	o.set_member("lastindexof", &string_last_index_of);
	o.set_member("substr", &string_sub_str);
	o.set_member("substring", &string_sub_string);
	o.set_member("indexOf", &string_index_of);
	o.set_member("toString", &string_to_string);
	o.set_member("fromCharCode", &string_from_char_code);
	o.set_member("charAt", &string_char_at);
	o.set_member("charCodeAt", &string_char_code_at);
	o.set_member("toUpperCase", &string_to_upper_case);
	o.set_member("toLowerCase", &string_to_lower_case);
}

static as_object*
getStringInterface()
{
	static boost::intrusive_ptr<as_object> o;
	if ( o == NULL )
	{
		o = new as_object();
		attachStringInterface(*o);
	}
	return o.get();
}

class tu_string_as_object : public as_object
{
public:
	tu_string m_string;

	tu_string_as_object()
		:
		as_object(getStringInterface())
	{
	}
	
	virtual bool get_member(const tu_stringi& name, as_value* val) {
		
		if (name == "length") 
		{
			val->set_int(m_string.utf8_length());
			return true;
		}
		
		return get_member_default(name,val);
	}

};

// all the arguments will be converted to string and concatenated
static void
string_concat(const fn_call& fn)
{
	tu_string this_string = ((tu_string_as_object*) fn.this_ptr)->m_string;
	
	int len = 0;
	for (int i = 0; i < fn.nargs; i++) len += strlen(fn.arg(i).to_string());
	
	char *newstr = new char[len + 1];
	int pos = 0;
	for (int i = 0; i < fn.nargs; i++) 
	{
		int len = strlen(fn.arg(i).to_string());
		memcpy((newstr + pos),fn.arg(i).to_string(),len);
		pos += len;
	}
	newstr[len] = '\0';
	
	tu_string returnstring(newstr);
	fn.result->set_tu_string(returnstring);
	delete[] newstr;
	
	//FIXME:  is the "delete newstr[];" okay ?
	// Because I don't know if tu_string copies newstr or not. Michael Meier 2006/11/21
}

// 1st param: start_index, 2nd param: end_index
static void
string_slice(const fn_call& fn)
{
	tu_string this_string = ((tu_string_as_object*) fn.this_ptr)->m_string;
	// Pull a slice out of this_string.
	int	start = 0;
	int	utf8_len = this_string.utf8_length();
	int	end = utf8_len;
	if (fn.nargs >= 1)
	{
		start = static_cast<int>(fn.arg(0).to_number());
		if (start < 0) start = utf8_len + start;
		start = iclamp(start, 0, utf8_len);
	}
	if (fn.nargs >= 2)
	{
		end = static_cast<int>(fn.arg(1).to_number());
		if (end < 0) end = utf8_len + end;
		end = iclamp(end, 0, utf8_len);
	}
	
	assert(end >= start);

	fn.result->set_tu_string(this_string.utf8_substring(start, end));
	return;
}

static void
string_split(const fn_call& fn)
{

	boost::intrusive_ptr<tu_string_as_object> this_string_ptr((tu_string_as_object*) fn.this_ptr);
	
	as_value val;
	
	boost::intrusive_ptr<as_array_object> array(new as_array_object());
	
	if (fn.nargs == 0) 
	{
		val.set_tu_string(this_string_ptr->m_string);
		array->push(val);
		
		fn.result->set_as_object(array.get());
		return;
	}
	
	if (fn.nargs >= 1)
	{
		tu_string this_string = ((tu_string_as_object*) fn.this_ptr)->m_string;
		
		int	utf8_len = this_string.utf8_length();

		if (strcmp("",fn.arg(0).to_string()) == 0)
		{
			for (int i = 0; i < utf8_len; i++) {
				val.set_tu_string(this_string.utf8_substring(i,i+1));
				array->push(val);
			}
			fn.result->set_as_object(array.get());
			return;
		}
		else 
		{
			const char *str = this_string.c_str();
			const char *delimeter = fn.arg(0).to_string();
			
			tu_string str_tu(str);
			tu_string delimeter_tu(str);
			
			int start = 0;
			int end;
			//int utf8_str_len = str_tu.utf8_length();
			//int utf8_delimeter_len = delimeter_tu.utf8_length();
			int delimeter_len = strlen(delimeter);
			
			const char *pstart = str;
			const char *pend = strstr(pstart,delimeter);
			while (pend != NULL)
			{
				//tu_string fromstart(pstart);
				//tu_string fromend(pend);
				start = tu_string::utf8_char_count(str,int(pstart-str));
				end = start + tu_string::utf8_char_count(pstart,int(pend-pstart));
				
				val.set_tu_string(this_string.utf8_substring(start,end));
				array->push(val);
				pstart = pend + delimeter_len;
				if (!(*pstart))
				{
					fn.result->set_as_object(array.get());
					return;
				}
				pend = strstr(pstart,delimeter);
				
			}
			val.set_tu_string(tu_string(pstart));
			array->push(val);
			fn.result->set_as_object(array.get());
			return;
		}
	}
	
}

static void
string_last_index_of(const fn_call& fn)
{
	tu_string_as_object* this_string_ptr = (tu_string_as_object*) fn.this_ptr;
	assert(this_string_ptr);

	if (fn.nargs < 1)
	{
		fn.result->set_double(-1);
		return;
	}
	else
	{
		int	start_index = 0;
		if (fn.nargs > 1)
		{
			start_index = static_cast<int>(fn.arg(1).to_number());
		}
		const char*	str = this_string_ptr->m_string.c_str();
		const char*	p = strstr(
			str + start_index,	// FIXME: not UTF-8 correct!
			fn.arg(0).to_string());
		if (p == NULL)
		{
			fn.result->set_double(-1);
			return;
		}
		
		const char* lastocc = p;
		while (p != NULL)
		{
			if (!(*p)) break;
			p = strstr((p+1),fn.arg(0).to_string()); // FIXME: also not UTF-8 correct!
			if (p) lastocc = p;
		}
		
		fn.result->set_double(tu_string::utf8_char_count(str, int(lastocc - str)));
		return;
	}
}

// 1st param: start_index, 2nd param: length (NOT end_index)
static void
string_sub_str(const fn_call& fn)
{
	tu_string this_string = ((tu_string_as_object*) fn.this_ptr)->m_string;
	// Pull a slice out of this_string.
	int	start = 0;
	int	utf8_len = this_string.utf8_length();
	int	end = utf8_len;
	if (fn.nargs >= 1)
	{
		start = static_cast<int>(fn.arg(0).to_number());
		if (start < 0) start = utf8_len + start;
		start = iclamp(start, 0, utf8_len);
	}
	if (fn.nargs >= 2)
	{
		end = static_cast<int>(fn.arg(1).to_number()) + start;
		end = iclamp(end, start, utf8_len);
	}

	fn.result->set_tu_string(this_string.utf8_substring(start, end));
	return;
}

// 1st param: start_index, 2nd param: end_index
static void
string_sub_string(const fn_call& fn)
{
	tu_string this_string = ((tu_string_as_object*) fn.this_ptr)->m_string;
	// Pull a slice out of this_string.
	int	start = 0;
	int	utf8_len = this_string.utf8_length();
	int	end = utf8_len;
	if (fn.nargs >= 1)
	{
		start = static_cast<int>(fn.arg(0).to_number());
		start = iclamp(start, 0, utf8_len);
	}
	if (fn.nargs >= 2)
	{
		end = static_cast<int>(fn.arg(1).to_number());
		end = iclamp(end, 0, utf8_len);
	}

	if (end < start) swap(&start, &end);	// dumb, but that's what the docs say
	assert(end >= start);

	fn.result->set_tu_string(this_string.utf8_substring(start, end));
	return;
}

static void
string_index_of(const fn_call& fn)
{
	tu_string_as_object* this_string_ptr = (tu_string_as_object*) fn.this_ptr;
	assert(this_string_ptr);

	if (fn.nargs < 1)
	{
		fn.result->set_double(-1);
		return;
	}
	else
	{
		int	start_index = 0;
		if (fn.nargs > 1)
		{
			start_index = static_cast<int>(fn.arg(1).to_number());
		}
		const char*	str = this_string_ptr->m_string.c_str();
		const char*	p = strstr(
			str + start_index,	// FIXME: not UTF-8 correct!
			fn.arg(0).to_string());
		if (p == NULL)
		{
			fn.result->set_double(-1);
			return;
		}

		fn.result->set_double(tu_string::utf8_char_count(str, p - str));
		return;
	}
}
 
static void
string_from_char_code(const fn_call& fn)
{
	tu_string_as_object* this_string_ptr = (tu_string_as_object*) fn.this_ptr;
	assert(this_string_ptr);

	tu_string result;

	for (int i = 0; i < fn.nargs; i++)
	{
		uint32 c = (uint32) fn.arg(i).to_number();
		result.append_wide_char(c);
	}

	fn.result->set_tu_string(result);
}

static void
string_char_code_at(const fn_call& fn)
{
	tu_string_as_object* this_string_ptr = (tu_string_as_object*) fn.this_ptr;
	assert(this_string_ptr);

	assert(fn.nargs == 1);

	int	index = static_cast<int>(fn.arg(0).to_number());
	if (index >= 0 && index < this_string_ptr->m_string.utf8_length())
	{
		fn.result->set_double(this_string_ptr->m_string.utf8_char_at(index));
		return;
	}

	double temp = 0.0;	// This variable will let us divide by zero without a compiler warning
	fn.result->set_double(temp/temp);	// this division by zero creates a NaN value
	return;
}

static void
string_char_at(const fn_call& fn)
{
	tu_string_as_object* this_string_ptr = (tu_string_as_object*) fn.this_ptr;
	assert(this_string_ptr);

	assert(fn.nargs == 1);

	int	index = static_cast<int>(fn.arg(0).to_number());
	if (index >= 0 && index < this_string_ptr->m_string.utf8_length())
	{
		tu_string result;
		result += this_string_ptr->m_string.utf8_char_at(index);
		fn.result->set_tu_string(result);
		return;
	}

	double temp = 0.0;	// This variable will let us divide by zero without a compiler warning
	fn.result->set_double(temp/temp);	// this division by zero creates a NaN value
	return;		
}

static void
string_to_upper_case(const fn_call& fn)
{
	tu_string_as_object* this_string_ptr = (tu_string_as_object*) fn.this_ptr;
	assert(this_string_ptr);

	fn.result->set_tu_string(this_string_ptr->m_string.utf8_to_upper());
	return;		
}

static void
string_to_lower_case(const fn_call& fn)
{
	tu_string_as_object* this_string_ptr = (tu_string_as_object*) fn.this_ptr;
	assert(this_string_ptr);

	fn.result->set_tu_string(this_string_ptr->m_string.utf8_to_lower());
	return;		
}

static void
string_to_string(const fn_call& fn)
{
	tu_string_as_object* this_string_ptr = (tu_string_as_object*) fn.this_ptr;
	assert(this_string_ptr);

	fn.result->set_tu_string(this_string_ptr->m_string);
}


static void
string_ctor(const fn_call& fn)
{
	boost::intrusive_ptr<tu_string_as_object> str = new tu_string_as_object;

	if (fn.nargs > 0)
	{
		str->m_string = fn.arg(0).to_tu_string();
	}
	
	// this shouldn't be needed
	//attachStringInterface(*str);

	fn.result->set_as_object(str.get());
}

// extern (used by Global.cpp)
void string_class_init(as_object& global)
{
	// This is going to be the global String "class"/"function"
	static boost::intrusive_ptr<builtin_function> cl;

	if ( cl == NULL )
	{
		cl=new builtin_function(&string_ctor, getStringInterface());
		// replicate all interface to class, to be able to access
		// all methods as static functions
		attachStringInterface(*cl);
		     
	}

	// Register _global.String
	global.set_member("String", cl.get());

}

std::auto_ptr<as_object>
init_string_instance(const char* val)
{
	tu_string_as_object* obj = new tu_string_as_object();
	if ( val ) obj->m_string = val;
	return std::auto_ptr<as_object>(obj);
}
  
} // namespace gnash
