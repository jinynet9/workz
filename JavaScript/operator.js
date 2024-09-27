'use strict';

/* Equality */
console.log('/* equality */');
const stringFive = '5';
const numberFive = 5;

// loose equality, with type conversion
console.log(stringFive == numberFive);
console.log(stringFive != numberFive);

// strict equality, no type conversion
console.log(stringFive === numberFive);
console.log(stringFive !== numberFive);

// object equality by reference
console.log('/* object equality */');
const ellie1 = { name: 'ellie' };
const ellie2 = { name: 'ellie' };
const ellie3 = ellie1;
console.log(ellie1 == ellie2);
console.log(ellie1 === ellie2);
console.log(ellie1 === ellie3);

// equality - puzzler
console.log('/* equality - puzzler */');
console.log(0 == false); // true
console.log(0 === false); // false
console.log('' == false); // true
console.log('' === false); // false
console.log(null == undefined); // true
console.log(null === undefined); // false

