-- interpit.lua
-- Jonathan Brough
-- Created: 2025-04-15
--
-- CS 331 Spring 2025 - Assignment 6A
-- Interpreter for Fulmar AST (from parseit.parse)

local interpit = {}

-- AST tag constants
local PROGRAM       = 1
local PRINT_STMT    = 2
local PRINTLN_STMT  = 3
local RETURN_STMT   = 4
local ASSN_STMT     = 5
local FUNC_CALL     = 6
local FUNC_DEF      = 7
local IF_STMT       = 8
local WHILE_LOOP    = 9
local STRLIT_OUT    = 10
local CHR_CALL      = 11
local BIN_OP        = 12
local UN_OP         = 13
local NUMLIT_VAL    = 14
local READNUM_CALL  = 15
local RND_CALL      = 16
local SIMPLE_VAR    = 17
local ARRAY_VAR     = 18

-- Utility functions
local function strToInt(str)
    local ok, val = pcall(function() return tonumber(str) end)
    return ok and val and math[ val >= 0 and "floor" or "ceil" ](val) or 0
end

local function intToStr(n)
    return tostring(n)
end

local function boolToInt(b)
    return b and 1 or 0
end

-- Main interpreter
function interpit.interp(ast, state, util)
    local function evalExpr(node)
        if node[1] == NUMLIT_VAL then
            return strToInt(node[2])
        elseif node[1] == READNUM_CALL then
            return strToInt(util.input())
        elseif node[1] == RND_CALL then
            return util.random(evalExpr(node[2]))
        elseif node[1] == SIMPLE_VAR then
            return state.v[node[2]] or 0
        elseif node[1] == ARRAY_VAR then
            local array = state.a[node[2]]
            local index = evalExpr(node[3])
            return (array and array[index]) or 0
        elseif node[1] == FUNC_CALL then
            execStmt(node)
            return state.v["return"] or 0
        elseif type(node[1]) == "table" and node[1][1] == BIN_OP then
            local lhs = evalExpr(node[2])
            local rhs = evalExpr(node[3])
            local op = node[1][2]
            if op == "+" then return lhs + rhs
            elseif op == "-" then return lhs - rhs
            elseif op == "*" then return lhs * rhs
            elseif op == "/" then return rhs ~= 0 and math[ lhs/rhs < 0 and "ceil" or "floor" ](lhs / rhs) or 0
            elseif op == "%" then return rhs ~= 0 and lhs % rhs or 0
            elseif op == "==" then return boolToInt(lhs == rhs)
            elseif op == "!=" then return boolToInt(lhs ~= rhs)
            elseif op == ">" then return boolToInt(lhs > rhs)
            elseif op == "<" then return boolToInt(lhs < rhs)
            elseif op == ">=" then return boolToInt(lhs >= rhs)
            elseif op == "<=" then return boolToInt(lhs <= rhs)
            elseif op == "&&" then return (lhs ~= 0 and rhs ~= 0) and 1 or 0
            elseif op == "||" then return (lhs ~= 0 or rhs ~= 0) and 1 or 0
            end
        elseif type(node[1]) == "table" and node[1][1] == UN_OP then
            local val = evalExpr(node[2])
            local op = node[1][2]
            if op == "+" then return val
            elseif op == "-" then return -val
            elseif op == "!" then return boolToInt(val == 0)
            end
        end
        return 0
    end

    local function evalPrintArg(arg)
        if arg[1] == STRLIT_OUT then
            return arg[2]:sub(2, -2)
        elseif arg[1] == CHR_CALL then
            local val = evalExpr(arg[2])
            return string.char((val >= 0 and val <= 255) and val or 0)
        else
            return intToStr(evalExpr(arg))
        end
    end

    function execStmt(stmt)
        local tag = stmt[1]
        if tag == PRINT_STMT or tag == PRINTLN_STMT then
            for i = 2, #stmt do
                util.output(evalPrintArg(stmt[i]))
            end
            if tag == PRINTLN_STMT then util.output("\n") end
        elseif tag == RETURN_STMT then
            state.v["return"] = evalExpr(stmt[2])
        elseif tag == ASSN_STMT then
            if stmt[2][1] == SIMPLE_VAR then
                state.v[stmt[2][2]] = evalExpr(stmt[3])
            else
                local name = stmt[2][2]
                local index = evalExpr(stmt[2][3])
                state.a[name] = state.a[name] or {}
                state.a[name][index] = evalExpr(stmt[3])
            end
        elseif tag == FUNC_DEF then
            state.f[stmt[2]] = stmt[3]
        elseif tag == FUNC_CALL then
            local funcAst = state.f[stmt[2]] or { PROGRAM }
            execBlock(funcAst)
        elseif tag == IF_STMT then
            for i = 2, #stmt, 2 do
                if stmt[i][1] == PROGRAM then
                    execBlock(stmt[i])
                elseif evalExpr(stmt[i]) ~= 0 then
                    execBlock(stmt[i + 1])
                    break
                end
            end
        elseif tag == WHILE_LOOP then
            while evalExpr(stmt[2]) ~= 0 do
                execBlock(stmt[3])
            end
        end
    end

    function execBlock(block)
        for i = 2, #block do
            execStmt(block[i])
        end
    end

    execBlock(ast)
    return state
end

return interpit