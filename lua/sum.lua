local CGraph = require 'CGraph'
local array = CGraph.array

local startTime = os.clock()

local B = CGraph.variable('b')
local C = CGraph.vector(9, array{1, 2, 3, 4, 5, 6, 7, 8, 9})

local PHI = CGraph.sum(B*CGraph.double(2), 0)

--local g1 = CGraph.graph("test", Z)
--local g2 = CGraph.graph("test", Y)
--local g3 = CGraph.graph("test", W)
local g4 = CGraph.graph("sum", PHI)

g4:setVar('b', CGraph.vector(9, array{1, 2, 3, 4, 5, 6, 7, 8, 9}))
print(g4:eval())
g4:backProp()
n = g4:getVarDiff('b')
print(n)
