{ -*- mode: pascal; pascal-indent-level: 2; compile-command: "fpc gol.pas && ./gol" -*- }
program gol;
const
  SIZE = 8;
  f    = 0;
  t    = 1;

type
  Plane = array[0..SIZE, 0..SIZE] of ShortInt;

var
  i,j,k : integer;
  front : Plane;
  back  : Plane;
  life  : array[0..1, 0..8] of ShortInt;

procedure PrintPlane(p : Plane);
var i, j : integer;
begin
  for j := 0 to SIZE do
  begin
    for i := 0 to SIZE do
    begin
      if p[i,j] = 1 then
        write('X')
      else
        write(' ');
    end;
    writeln();
  end
end;

function Count(p : Plane; x : Integer; y : Integer ): Integer;
var
  dx, dy : Integer;
  ret    : Integer = 0;
begin
  for dy := -1 to 1 do
    for dx := -1 to 1 do
    begin
      if not ((dx = dy) and (dx = 0)) then
        if (x+dx in [0..SIZE]) and (y+dy in [0..SIZE]) then
          if p[x+dx, y+dy] = 1 then
            Inc(ret)
    end;

  Count := ret;
end;

begin
  for i in [0..2] do
    for j in [0..8] do
      life[i,j] := 0;

  for i in [0..SIZE] do
    for j in [0..SIZE] do
      front[i,j] := 0;

  life[0,3] := 1;
  life[1,2] := 1;
  life[1,3] := 1;

  front[1,0] := 1;
  front[2,1] := 1; {  #  }
  front[0,2] := 1; {   # }
  front[1,2] := 1; { ### }
  front[2,2] := 1;

  for i := 0 to 10 do
  begin
    PrintPlane(front);
    for j := 0 to SIZE do
      for k := 0 to SIZE do
      begin
        back[k, j] := life[front[k,j], Count(front, k, j)];
      end;
    front := back
  end;
end.
