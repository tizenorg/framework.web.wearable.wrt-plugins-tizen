
if( ref.syncToSync(1,1) == 2 )
	console.log("PASS 1");
else
	console.log("FAIL 1");
	
if( ref.syncToSync(1,2) == 3 )
	console.log("PASS 2");
else
	console.log("FAIL 2");
	

try{
	ref.syncToSync(5,1);
	console.log("FAIL 3");
}catch(e){
	console.log("PASS 3");
}	

try{
	ref.syncToSync(4,40);
	console.log("FAIL 4");
}catch(e){
	console.log("PASS 4");
}	

ref.syncToAsync(1,2 ,function(r){ if(r == 3)console.log("PASS 5");else console.log("FAIL 5");}, function(e){ console.log("FAIL 5");});
ref.syncToAsync(3,2 ,function(r){ if(r == 5)console.log("PASS 6");else console.log("FAIL 6");}, function(e){ console.log("FAIL 6");});

try{
	ref.syncToAsync(5,1, function(r){ console.log("FAIL 7"); }, function(e){ console.log("FAIL 7");});
	console.log("FAIL 7");
}catch(e){
	console.log("PASS 7");
}

ref.syncToAsync(4,40 ,function(r){ console.log("FAIL 8");}, function(e){ console.log("PASS 8");});


ref.asyncToAsync(1,2 ,function(r){ if(r == 3)console.log("PASS 9");else console.log("FAIL 9 "+r);}, function(e){ console.log("FAIL 9(2)");});
ref.asyncToAsync(3,2 ,function(r){ if(r == 5)console.log("PASS 10");else console.log("FAIL 10 "+r);}, function(e){ console.log("FAIL 10(2)");});

try{
	ref.asyncToAsync(5,1, function(r){ console.log("FAIL 11"); }, function(e){ console.log("FAIL 11");});
	console.log("FAIL 11");
}catch(e){
	console.log("PASS 11");
}

ref.asyncToAsync(4,40 ,function(r){ console.log("FAIL 12");}, function(e){ console.log("PASS 12");});


