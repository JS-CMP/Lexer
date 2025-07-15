var test = true;

console.log(new (test ? String : Object)()); // NEW((test ? String : Object))
