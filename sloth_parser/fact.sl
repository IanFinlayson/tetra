% program to calculate factorials

% get input
x := input;
fact := 1;

% a loop!
while x > 1.0 do begin
  fact := fact * x;
  x := x - 1;
end

% output
print fact;

