'use strict';

/*

variable 선언
- mutable type (let)
- immutable type (const)

variable type
. primitive (single item): number, string, boolean, null, undefined, symbol
. object (box container)
. function (first-class function)

type이 dynamic하게 결정됨
let a = 12;
let b: number = 1.2;
*/

/*
 * number
 *
-2**53 ~ 2*53

special number value:
. infinity = 1 / 0;
. negativeInfinity = -1 / 0;
. NaN = 'not a number' / 2;

*/
console.log('/* number */');
const infinity = 1 / 0;
const negativeInfinity = -1 / 0;
const notAnumber = 'not a number' * 1;

console.log(infinity); /* Infinity */
console.log(negativeInfinity); /* -Infinity */
console.log(notAnumber); /* NaN */


/*
 * string
 */
console.log('/* string */');
const char = 'c';
const brendan = 'brendan';
const greeting = 'hello ' + brendan;
console.log(`value: ${greeting}, type: ${typeof greeting}`);

const helloBob = `hi ${brendan}!`; // template literals (string)
console.log(`value: ${helloBob}, type: ${typeof helloBob}`);

/*
 * boolean
 * false: 0, null, undefined, NaN, ''
 * true: any other value
 */
console.log('/* boolean */');
const canRead = true;
const test = 3 < 1; // false
console.log(`value: ${canRead}, type: ${typeof canRead}`);
console.log(`value: ${test}, type: ${typeof test}`);

/*
 * null
 */
console.log('/* null */');
let nothing = null;
console.log(`value: ${nothing}, type: ${typeof nothing}`);

/*
 * undefined
 */
console.log('/* undefined */');
let x;
let y = undefined;
console.log(`value: ${x}, type: ${typeof x}`);
console.log(`value: ${y}, type: ${typeof y}`);

/*
 * symbol
 * create unique identifiers for objects
 */
console.log('/* symbol */');
const symbol1 = Symbol('id');
const symbol2 = Symbol('id');
console.log(symbol1 === symbol2 );
console.log(`value: ${symbol1.description}, type: ${typeof symbol1}`);
const symbol11 = Symbol.for('id');
const symbol22 = Symbol.for('id');
console.log(symbol11 === symbol22 );

/*
 * object
 * real-life object, data structure
 */
console.log('/* object */');
const ellie = { name: 'ellie', age: 20 };
ellie.age = 21;
console.log(`value: ${ellie}, type: ${typeof ellie}`);
console.log(`value: ${ellie.age}, type: ${typeof ellie.age}`);

/*
 * Dynamic typing
 * dynamically typed language
 */
console.log('/* dynamic typing */');
let text = 'hello';
console.log(`value: ${text}, type: ${typeof text}`);
console.log(text.charAt(0)); // h
text = 1;
console.log(`value: ${text}, type: ${typeof text}`);
text = '7' + 5;
console.log(`value: ${text}, type: ${typeof text}`);
text = '8' / '2';
console.log(`value: ${text}, type: ${typeof text}`);
console.log(text.charAt(0)); // TypeError



