// 
//   Copyright (C) 2005, 2006, 2007 Free Software Foundation, Inc.
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


// Test case for ActionScript inheritance
// compile this test case with Ming makeswf, and then
// execute it like this gnash -1 -r 0 -v out.swf

rcsid="$Id: Inheritance.as,v 1.34 2007/08/16 09:29:33 zoulunkai Exp $";

#include "check.as"

check_equals(typeof(Object.prototype.constructor), 'function');
check_equals(Object.prototype.constructor, Object);

// Function.apply is Function.__proto__.apply
#if OUTPUT_VERSION > 5
check (typeof(Function.apply) != undefined);
#endif
check (Function.apply == Function.__proto__.apply);

// Confirm '__proto__' and 'prototype' members
// for Function to be the same thing
Function.prototype.fake = function(){};
check(Function.fake == Function.__proto__.fake);
check(Function.fake == Function.prototype.fake);

// Make 'functionObject' be an instance of Function (an object)
var functionObject = new Function();

#if OUTPUT_VERSION > 5
check_equals(typeof(functionObject), 'object');
check(functionObject.hasOwnProperty('__constructor__'));
check_equals(functionObject.__constructor__, Function);
#else
// TODO: this is likely dependent on *player* version 
//       rather then on *SWF* version, in which case
//       we should completely avoid testing it.
//       Can anyone confirm ?
xcheck_equals(typeof(functionObject), 'undefined');
#endif

// functionObject '__proto__' is a reference to
// it's constructor's 'prototype' member.
check_equals (functionObject.__proto__, Function.prototype);
check_equals (functionObject.__proto__.constructor, Function);

// functionObject.apply should be functionObject.__proto__.apply
#if OUTPUT_VERSION > 5
check (functionObject.apply != undefined);
check (functionObject.apply == Function.prototype.apply);
check (functionObject.apply == functionObject.__proto__.apply);
#endif

// functionObject is an Object, not functionObject Function,
// so it doesn't have functionObject 'prototype' member
// itself.
check (functionObject.prototype == undefined);

#if OUTPUT_VERSION > 5
functionObject.prototype = {a:1};
check_equals(functionObject.prototype.a, 1);
#endif

// Make 'userFunc' be a function (should inherit Function things)
var userFunc = function() {};
check_equals (userFunc.__proto__, Function.prototype);
check_equals (userFunc.prototype.constructor, userFunc);
check_equals (userFunc.prototype.apply, undefined);
check_equals (userFunc.apply, Function.prototype.apply);

// Override the inherited apply() method
#if OUTPUT_VERSION > 5 
// Function.apply was introduced in SWF6
userFunc.apply = function() {};
check (userFunc.apply != Function.prototype.apply);
#endif


// Define the Ball constructor
function Ball (radius) {
	this.radius = radius;
}
check (Ball.prototype.constructor == Ball);
Ball.prototype.gravity = 9.8;

var myBall = new Ball(3);
#if OUTPUT_VERSION > 5
check(myBall.hasOwnProperty('__constructor__'));
check_equals(myBall.__constructor__, Ball);
#endif
check (myBall.gravity == 9.8);
check(myBall.radius == 3);

function sayHello() { return "Hello"; }
Ball.prototype.sayHello = sayHello;
check(myBall.sayHello() == "Hello");
check(myBall.gravity == 9.8);
myBall.gravity = 5;
check(myBall.gravity == 5);
check(myBall.__proto__ == Ball.prototype);

check(!_root.hasOwnProperty('__constructor__'));

//-------------------------------------------------------------------------
// Try changing value of __constructor__ from within the constructor itself
//-------------------------------------------------------------------------

function TypeChanger(changeit)
{
	if ( changeit ) this.__constructor__ = Object;
}

o1 = new TypeChanger(false);
#if OUTPUT_VERSION > 5
check_equals(o1.__constructor__, TypeChanger);
#else
xcheck_equals(o1.__constructor__, TypeChanger);
#endif
check(o1 instanceof TypeChanger);
check(o1 instanceof Object);
o2 = new TypeChanger(true);
check_equals(o2.__constructor__, Object);
check(o2 instanceof TypeChanger);
check(o2 instanceof Object);

//-------------------------------------------------------------------------
// Old-style inheritance
//-------------------------------------------------------------------------

// Define a superclass
function SuperClass() {
	this.sayHello = function() { return "hello from SuperClass"; };
}

// Define a class derived from SuperClass
function SubClass () {}
check_equals(SubClass.prototype.constructor, SubClass);
check_equals(typeof(SubClass.prototype.__constructor__), 'undefined');

SubClass.prototype = new SuperClass();
check_equals(SubClass.prototype.constructor, SuperClass);

subInstance = new SubClass();
#if OUTPUT_VERSION > 5
check(subInstance.hasOwnProperty('__constructor__'));
check_equals(subInstance.__constructor__, SubClass);
check_equals(subInstance.__proto__.constructor, SuperClass);
#endif

check_equals(subInstance.sayHello(), "hello from SuperClass" );
SubClass.prototype.sayHello = function() { return "hello from SubClass"; };
check_equals(subInstance.sayHello(), "hello from SubClass" );
subInstance.sayHello = function() { return "hello from subInstance"; };
check_equals(subInstance.sayHello(), "hello from subInstance" );

// Test the instanceOf operator
check(subInstance instanceOf SubClass);
check(subInstance instanceOf SuperClass);
check(subInstance instanceOf Object);


//------------------------------------------------
// Test the 'super' keyword
//------------------------------------------------

#if OUTPUT_VERSION > 5
function BaseClass() {}
BaseClass.prototype.sayHello = function () {
  return "Hello from BaseClass"; 
};
function DerivedClass() {}
DerivedClass.prototype = new BaseClass();
DerivedClass.prototype.sayHello = function () {
  return "Hello from DerivedClass"; 
};
DerivedClass.prototype.saySuperHello = function () {
  return super.sayHello();
};
var derived = new DerivedClass();
var greeting = derived.saySuperHello();
check_equals(greeting, "Hello from BaseClass");
#endif // OUTPUT_VERSION > 5
check_equals(super, undefined);

//------------------------------------------------
//
//------------------------------------------------


/// THese have been moved here from inheritance.as
var obj = new Object({a:1});

check_equals(obj.__proto__.constructor, Object);
check(obj instanceOf Object);

function SubObj1() {}
var sobj1 = new SubObj1();

check_equals(sobj1.__proto__.constructor, SubObj1);
#if OUTPUT_VERSION > 5
check(SubObj1 instanceOf Function);
check(Function instanceOf Object);
check(SubObj1 instanceOf Object);
#endif

// inheritance chain is NOT subobj1,SubObj1,Function,Object, as the
// above might suggest...
check(!sobj1 instanceOf Function);

// but still, sobj1 is an instance of Object *and* SubObj1
check(sobj1 instanceOf Object);
check(sobj1 instanceOf SubObj1);

check(SubObj1.prototype != undefined);
check_equals(SubObj1.prototype.constructor, SubObj1);

check_equals(SubObj1.prototype.constructor.__proto__.constructor, Function);

//------------------------------------------------
// Test the 'extends' tag (require ming > 0.4.0.beta3)
//------------------------------------------------

// see check.as
#ifdef MING_SUPPORTS_ASM_EXTENDS

function BaseClass1() { this.baseClassCtorCalled = 1; }
BaseClass1.prototype.var1 = "var_in_Base_prototype";
function DerivedClass1() { this.derivedClassCtorCalled = 1; }
DerivedClass1.prototype.var3 = "var3_in_Derived_prototype";
asm {
	push "DerivedClass1"
	getvariable
	push "BaseClass1"
	getvariable
	extends
};

// One effect of 'extends' is setting up a __constructor__ member in the prototype 
#if OUTPUT_VERSION > 5
check_equals(typeof(DerivedClass1.prototype.__constructor__), 'function');
check(DerivedClass1.prototype.hasOwnProperty('__constructor__'));
check_equals(DerivedClass1.prototype.__constructor__, BaseClass1);
check(DerivedClass1.prototype.__constructor__ != DerivedClass1);
#else // SWF5 or below don't set __constructor__, it seems
check_equals(typeof(DerivedClass1.prototype.__constructor__), 'undefined');
#endif

check_equals(typeof(DerivedClass1.prototype.constructor), 'function');
check_equals(typeof(DerivedClass1.constructor), 'function');
#if OUTPUT_VERSION >= 6
xcheck(DerivedClass1.prototype.hasOwnProperty('__proto__'));
check(DerivedClass1.prototype.hasOwnProperty('__constructor__'));
check(DerivedClass1.prototype.__proto__.hasOwnProperty('var1'));

check(! DerivedClass1.prototype.hasOwnProperty('var1'));
check(! DerivedClass1.prototype.hasOwnProperty('var3'));
check_equals(DerivedClass1.var3, undefined);
check(! DerivedClass1.prototype.hasOwnProperty('toString'));
check(! DerivedClass1.prototype.hasOwnProperty('valueOf'));
xcheck(! DerivedClass1.prototype.hasOwnProperty('constructor'));
check(DerivedClass1.hasOwnProperty('constructor'));
#endif
check_equals(DerivedClass1.constructor, Function);
check_equals(DerivedClass1.prototype.__proto__.constructor, BaseClass1);
//check_equals(DerivedClass1.prototype.constructor, BaseClass1);

DerivedClass1.prototype.var2 = "var_in_Derived_prototype";
var obj = new DerivedClass1;
check_equals(obj.derivedClassCtorCalled, 1);
#if OUTPUT_VERSION > 5
check_equals(typeof(obj.__constructor__), 'function');
check(obj.hasOwnProperty('__constructor__'));
check_equals(obj.__constructor__, DerivedClass1);
check_equals(obj.__proto__.__constructor__, BaseClass1);
#endif

// constructor of 'super' is not automatically called
// add 'super();' in DerivedClass1 function and see
// the difference
check_equals(obj.baseClassCtorCalled, undefined);
check(obj instanceOf DerivedClass1);
check(obj instanceOf BaseClass1);
check_equals(obj.__proto__, DerivedClass1.prototype);
check_equals(DerivedClass1.prototype.constructor, BaseClass1);
check_equals(obj.var1, "var_in_Base_prototype");
check_equals(obj.var2, "var_in_Derived_prototype");
check_equals(typeof(obj.var3), 'undefined');

function MyClass() {}
var proto = MyClass.prototype;
MyClass.prototype.oldMember = "overridden";
asm {
	push "MyClass"
	getvariable
	push "MovieClip"
	getvariable
	extends
};
var myInstance = new MyClass();
check(_root instanceOf MovieClip);
check_equals(MyClass.prototype.constructor, MovieClip);
check_equals(myInstance.__proto__, MyClass.prototype);
check_equals(typeof(MovieClip.prototype._x), 'undefined');
check_equals(typeof(myInstance.oldMember), 'undefined');
check_equals(proto.oldMember, 'overridden');

#endif // MING_SUPPORTS_ASM_EXTENDS

//------------------------------------------------
// Test access of builtin methods or getter/setter
// from a user-defined instance.
//------------------------------------------------

function Test() {}
check_equals(typeof(Date.prototype), 'object');
Test.prototype = new Date();
var t = new Test;
check_equals(typeof(t.getYear), 'function');
check_equals(typeof(t.setYear), 'function');
t.setYear(2007);
check_equals(typeof(t.getYear()), 'undefined');

var t2 = new Object;
t2.__proto__ = Date.prototype;
check_equals(typeof(t2.getYear), 'function');
check_equals(typeof(t2.setYear), 'function');
t2.setYear(2007);
check_equals(typeof(t2.getYear()), 'undefined');

var t3 = new Object;
t3.__proto__ = MovieClip.prototype;
check_equals(typeof(t4.gotoAndStop), 'undefined');
check_equals(typeof(t3._currentframe), 'undefined');
t3._currentframe = 10;
check_equals(typeof(t3._currentframe), 'number');

function Test4() {}
Test3.prototype = new MovieClip;
var t4 = new Test4;
check_equals(typeof(t4.gotoAndStop), 'undefined');
check_equals(typeof(t4._currentframe), 'undefined');
t4._currentframe = 10;
check_equals(typeof(t4._currentframe), 'number');

check_equals(typeof(_root.gotoAndPlay), 'function');
t4.die = _root.gotoAndPlay;
check_equals(typeof(t4.die), 'function');
var b = t4.die(4);
check_equals(typeof(b), 'undefined');

//------------------------------------------------
// Test circular inheritance
//------------------------------------------------

function Test5() {}
function Test4() {}
Test5.__proto__ = Test4.prototype;
Test4.__proto__ = Test5.prototype;
var t5 = new Test5; 
var t4 = new Test4;

check(t5 instanceOf Test5);
check(! t5 instanceOf Test4);
check(t4 instanceOf Test4);
check(! t4 instanceOf Test5);
