require 'continuation'

$c = nil

def fib
  a, b = [0, 1]
  callcc { |c| $c = c }
  # この位置に継続をマーキング
  puts b
  # フィボナッチ更新
  if b < 100 then
    tmp = b
    b = a + tmp
    a = tmp
    $c.call
  end
end

fib
