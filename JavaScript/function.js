
'use strict';

console.log(`Current Node.js version is: ${process.version}`);

/*
 * function is object in JS
 */

/*
 * parameters
 * premitive parameters: pass by value
 * object parameters: pass by reference
 */
function changeName(obj) {
    obj.name = 'coder';
}
const ellie = { name: 'ellie' };
console.log(ellie);
changeName(ellie);
console.log(ellie);

/*
 * Default parameters (added in ES6)
 * Default return: return undefined;
 */
function showMessage(message, from = 'unknown') {
    console.log(`${message} by ${from}`);
}
console.log('// Default parameter //');
showMessage('Hi!');

/*
 * Rest parameters (added in ES6)
 * pass by array
 */
function printAll(...args) {
	/*
    for (let i = 0; i < args.length; i++ )
	console.log(args[i]);

    for (const arg of args)
	console.log(arg);
	*/

    args.forEach((arg) => console.log(arg));
}
console.log('// Rest parameter //');
showMessage('dream', 'coding', 'ellie');

/*
 * Eary return, early exit
 */
function upgradeUser(user) {
    /* bad */
    if (user.point > 10) {
	// upgrade logic
    }

    /* good */
    if (user.point <= 10) {
	return;
    }

    // upgrade logic
}

/*
 * First-class function
 * functions are treated like any other variable
 * can be assigned as a value to variable
 * can be passed as an argument to other functions
 * can be returned by another function
 */


/*
 * Function expression
 * a function declaration can be called earlier than it is defined. (hoisted)
 * a function expression is created when the excution reaches it.
 */
console.log('// Function expression //');
console.log(sum(2,3));
function sum(a,b) {
    return a + b;
}

const print = function () {
    console.log('print');
};
print();
const printAgain = print;
printAgain();

/*
 * Arrow function
 * replace 'function' keyword with '=>'
 */
console.log('// Arrow function //');
const add1 = (a, b) => {
    return a + b;
}
const add2 = (a, b) => a + b;
console.log(add1(2,3));
console.log(add2(5,3));

/*
 * IIFE: Immediately Invoked Function Expression
 */
console.log('// IIFE //');
(function hello() {
    console.log('IIFE');
})();


