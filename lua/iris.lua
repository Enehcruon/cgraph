local CGraph = require 'CGraph'
local array = CGraph.array

local FileReader = require 'FileReader'
local _ = require 'underscore'
local cg = CGraph

local crossEntroy = CGraph.crossEntropyLoss

classes= {}

classes["Iris-setosa"] = 0
classes["Iris-versicolor"] = 1
classes["Iris-virginica"] = 2

X = {}
y = {}

local fn = function(line)
  local id, sl, sw, pl, pw, c = line:match('(%S+),(%S+),(%S+),(%S+),(%S+),(%S+)')
  table.insert(X, {tonumber(sl), tonumber(sw), tonumber(pl), tonumber(pw)})
  table.insert(y, classes[c])
end

FileReader.read("Iris.csv", fn)

table.remove(X, 1)

local Xinput = X
local Yclass = y


function shuffle(tbl, tbl2)
  size = #tbl
  for i = size, 1, -1 do
    local rand = math.random(i)
    tbl[i], tbl[rand] = tbl[rand], tbl[i]
    tbl2[i], tbl2[rand] = tbl2[rand], tbl2[i]
  end
  return tbl, tbl2
end


local function sigmoid(Z)
  local sigmoid = CGraph.double(1) / (CGraph.double(1) + CGraph.exp(-Z))
  return sigmoid
end


local function softmax(Z)
	return CGraph.exp(Z) / CGraph.sum(CGraph.exp(Z), 0)
end



local X = CGraph.variable 'X'
local theta1 = CGraph.variable 'T_1'
local b1 = CGraph.variable 'b_1'
local theta2 = CGraph.variable 'T_2'
local b2 = CGraph.variable 'b_2'
local theta3 = CGraph.variable 'T_3'
local b3 = CGraph.variable 'b_3'
local y = CGraph.variable 'y'

local A2 = sigmoid(CGraph.dot(X, theta1) + b1)
local A3 = sigmoid(CGraph.dot(A2, theta2) + b2)
local final = sigmoid(CGraph.dot(A3, theta3) + b3)

local eval = softmax(final)

local g = CGraph.graph("nn", crossEntroy((final), y, 3))

g:plot()

function gaussian (mean, variance)
    math.randomseed(os.time()+math.random()*1000)
    local v1  = math.sqrt(-2 * variance * math.log(math.random()))
    math.randomseed(os.time()+math.random()*3000)
    local v2 = math.cos(2 * math.pi * math.random()) + mean
    return v1 * v2
end

function randomWeight(size)
  vec = {}
  for i=1,size do
    table.insert(vec, gaussian(0, 0.01))
  end
  
  return vec
end

function updateWeights(name)
  local t_1 = g:getVar(name)
  local dxt_1 = g:getVarDiff(name)
  local alpha = 0.1
  
  local size = t_1.len or t_1.cols*t_1.rows
  
  local t_1_newval = {}
  for k=1,size do
    table.insert(t_1_newval, t_1.value[k] - alpha*dxt_1.value[k])
  end
  
  if t_1.len then
    g:setVar(name, CGraph.vector(size, t_1_newval))
  else
    g:setVar(name, CGraph.matrix(t_1.rows, t_1.cols, t_1_newval))
  end
  
end

lossPerClass = {0,0,0}

function argmax(t)
  local max, max_idx = t[1], 1
  for i,v in ipairs(t) do
    if v > max then
      max = v
      max_idx = i
    end
  end

  return max, max_idx
end

function buildConfusionMatrix(nb_classes)
  local mat = {}
  for i=1,nb_classes do
    mat[i] = {}
    for j=1,nb_classes do
      mat[i][j] = 0
    end
  end

  return mat
end

function train(X, y, X_test, Y_test)
  print('Training on '..#X..' samples and testing on '..#X_test..' samples.')
  g:setVar('T_1', CGraph.matrix(4, 5, randomWeight(20)))
  g:setVar('b_1', CGraph.vector(5, randomWeight(5)))
  g:setVar('T_2', CGraph.matrix(5, 5, randomWeight(25)))
  g:setVar('b_2', CGraph.vector(5, randomWeight(5)))
  g:setVar('T_3', CGraph.matrix(5, 3, randomWeight(15)))
  g:setVar('b_3', CGraph.vector(3, randomWeight(3)))
 --[[
  print(g:getVar('T_1'))
  print(g:getVar('b_1'))
  print(g:getVar('T_2'))
  print(g:getVar('b_2'))
  print(g:getVar('T_3'))
  print(g:getVar('b_3'))
  ]]
  g:setVar('X', CGraph.matrix(1, 4, _.flatten({X[i]})))
  g:setVar('y', CGraph.vector(1, _.flatten({y[i]})))
 
  local output = g:eval()
 
  
  loss = {}
  for k=1,1000 do
    local err = 0
    X, y = shuffle(X, y)
    for i=1,#X,1 do
      g:setVar('X', CGraph.matrix(1, 4, _.flatten({X[i]})))
      g:setVar('y', CGraph.vector(1, _.flatten({y[i]})))
      local output = g:eval()
      table.insert(loss, output.value)
      --print(output.value)
      
      g:backProp()
      
      --local dxt_1 = g:getVarDiff('b_1')
      --print(dxt_1)

      updateWeights('T_1')
      updateWeights('b_1')
      updateWeights('T_2')
      updateWeights('b_2')
      updateWeights('T_3')
      updateWeights('b_3')
      
   end
  end
  print(loss[#loss-1])
  local confMat = buildConfusionMatrix(3)
  for i=1,#X_test,1 do
    g:setVar('X', CGraph.matrix(1, 4, _.flatten({X_test[i]})))
    g:setVar('y', CGraph.vector(1, _.flatten({Y_test[i]})))
    local output = g:evalNode(eval)
    --print(output)
    --print(y[i])
    local max, idx = argmax(output.value)
    confMat[idx][Y_test[i]+1] = confMat[idx][Y_test[i]+1] + 1
  end

  for i=1,3 do
    print(confMat[i][1], confMat[i][2], confMat[i][3])
  end
end

Xinput, Yclass = shuffle(Xinput,Yclass)
Xinput, Yclass = shuffle(Xinput,Yclass)
Xinput, Yclass = shuffle(Xinput,Yclass)
Xinput, Yclass = shuffle(Xinput,Yclass)


train(_.initial(Xinput, 100), _.initial(Yclass, 100), _.last(Xinput, 50), _.last(Yclass, 50))
  
--[[
g:backProp()

print 'T_1'
print(g:getVarDiff('T_1'))
]]
