--parseit.lua 
--Jonathan Brough
--02/27/25
--Parser Module in Lua for Assignment 4

local lexit = require "lexit"

local parseit = {}

-- *********************************************************************
-- Variables
-- *********************************************************************

-- For lexer iteration
local iter          -- Iterator returned by lexit.lex
local state         -- State for above iterator (maybe not used)
local lexer_out_s   -- Return value #1 from above iterator
local lexer_out_c   -- Return value #2 from above iterator

-- For current lexeme
local lexstr = ""   -- String form of current lexeme
local lexcat = 0    -- Category of current lexeme:
                    --  one of categories below, or 0 for past the end

-- For last matched lexeme
local matched = ""  -- String form of last matched lexeme

-- *********************************************************************
-- Symbolic Constants for AST
-- *********************************************************************

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


local function advance()
    -- Advance the iterator
    lexer_out_s, lexer_out_c = iter(state, lexer_out_s)

    -- If we're not past the end, copy current lexeme into vars
    if lexer_out_s ~= nil then
        lexstr, lexcat = lexer_out_s, lexer_out_c
    else
        lexstr, lexcat = "", 0
    end
end

-- init
-- Initial call. Sets input for parsing functions.
local function init(prog)
    iter, state, lexer_out_s = lexit.lex(prog)
    advance()
end

-- atEnd
-- Return true if pos has reached end of input.
-- Function init must be called before this function is called.
local function atEnd()
    return lexcat == 0
end

-- matchString
-- Given string, see if current lexeme string form is equal to it. If
-- so, then set "matched" to the matched string, advance to next lexeme
-- & return true. If not, then do not advance, return false.
-- Function init must be called before this function is called.
local function matchString(s)
    if lexstr == s then
        matched = lexstr
        advance()
        return true
    else
        return false
    end
end


-- matchCat
-- Given lexeme category (integer), see if current lexeme category is
-- equal to it. If so, then set "matched" to the string form of the
-- matched lexeme, advance to next lexeme & return true. If not, then do
-- not advance, return false.
-- Function init must be called before this function is called.
local function matchCat(c)
    if lexcat == c then
        matched = lexstr
        advance()
        return true
    else
        return false
    end
end

-- *********************************************************************
-- "local" Statements for Parsing Functions
-- *********************************************************************

local parse_program
local parse_statement
local parse_print
local parse_expr
local parse_compare
local parse_arith
local parse_term
local parse_factor

-- *********************************************************************
-- The Parser: Function "parse" - EXPORTED
-- *********************************************************************

function parseit.parse(prog)
    -- Initialization
    init(prog)

    -- Get results from parsing
    local good, ast = parse_program()  -- Parse start symbol
    local done = atEnd()

    -- And return them
    return good, done, ast
end

function parse_program()  -- 1. Program
    local good, ast, newast
    ast = {PROGRAM}

    while lexstr == "print" or lexstr == "println" or lexstr == "return" or
            lexcat == lexit.ID or lexstr == "if" or lexstr == "while" or 
            lexstr == "func" do
        good, newast = parse_statement()
        if not good then
            return false, nil
        end
        table.insert(ast, newast)
    end
    return true, ast
end

function parse_statement()
    local good, ast, stmt, savelex, newast, nnewast

    -- 2. Print/Println Statement
    if matchString("print") or matchString("println") then
        if matched == "print" then
            stmt = {PRINT_STMT}
        else
            stmt = {PRINTLN_STMT}
        end
        if matchString("(") then
            if matchString(")") then
                return true, stmt
            end
            good, ast = parse_print()
            if not good then
                return false, nil
            end
            table.insert(stmt, ast)
            while matchString(",") do
                good, ast = parse_print()
                if not good then
                    return false, nil
                end
                table.insert(stmt, ast)
            end
            if matchString(")") then
                return true, stmt
            end
        end

    -- 3. Return Statement
    elseif matchString("return") then
        good, ast = parse_expr()
        if not good then
            return false, nil
        end
        return true, {RETURN_STMT, ast}

    -- 4. Statement Beginning with Identifier
    elseif matchCat(lexit.ID) then
        savelex = matched
        if matchString("(") and matchString(")") then
            return true, {FUNC_CALL, savelex}
        elseif matchString("[") then
            good, ast = parse_expr()
            if not good then
                return false, nil
            end
            if not matchString("]") then
                return false, nil
            end
            if matchString("=") then
                good, newast = parse_expr()
                if not good then
                    return false, nil
                end
                return true, {ASSN_STMT, {ARRAY_VAR, savelex, ast}, newast}
            end
        elseif matchString("=") then
            good, ast = parse_expr()
            if not good then
                return false, nil
            end
            return true, {ASSN_STMT, {SIMPLE_VAR, savelex}, ast}
        end

    -- 5. Function Definition
    elseif matchString("func") then
        if matchCat(lexit.ID) then
            savelex = matched
            if matchString("(") and matchString(")") then
                good, ast = parse_program()
                if not good then
                    return false, nil
                end
                if matchString("end") then
                    return true, {FUNC_DEF, savelex, ast}
                end
            end
        end

    -- 6. If Statement
    elseif matchString("if") then
        good, ast = parse_expr()
        if not good then
            return false, nil
        end
        good, newast = parse_program()
        if not good then
            return false, nil
        end
        ast = {IF_STMT, ast, newast}
        while matchString("elif") do
            good, newast = parse_expr()
            if not good then
                return false, nil
            end
            good, nnewast = parse_program()
            if not good then
                return false, nil
            end
            table.insert(ast, newast)
            table.insert(ast, nnewast)
        end
        if matchString("else") then
            good, newast = parse_program()
            if not good then
                return false, nil
            end
            table.insert(ast, newast)
        end
        if matchString("end") then
            return true, ast
        end

    -- 7. While Loop
    elseif matchString("while") then
        good, ast = parse_expr()
        if not good then
            return false, nil
        end
        good, newast = parse_program()
        if not good then
            return false, nil
        end
        if matchString("end") then
            return true, {WHILE_LOOP, ast, newast}
        end
    end

    return false, nil
end

-- 8. Print-Argument: StringLiteral
function parse_print()
    local good, ast
    if matchCat(lexit.STRLIT) then
        return true, {STRLIT_OUT, matched}

    -- 9. Output-Argument: Character Code
    elseif matchString("chr") and matchString("(") then
        good, ast = parse_expr()
        if not good then return false, nil end
        if not matchString(")") then return false, nil end
        return true, {CHR_CALL, ast}

    -- 10. Output-Argument: Expression
    else
        good, ast = parse_expr()
        if not good then return false, nil end
        return true, ast
    end
end

-- 11. Expression
function parse_expr()
    local good, ast, saveop, newast
    good, ast = parse_compare()
    if not good then return false, nil end

    while matchString("||") or matchString("&&") do
        saveop = matched
        good, newast = parse_compare()
        if not good then return false, nil end
        ast = {{BIN_OP, saveop}, ast, newast}
    end 
    return true, ast
end

-- 12. Comparison Expression
function parse_compare()
    local good, ast, saveop, newast
    good, ast = parse_arith()
    if not good then return false, nil end

    while matchString("==") or matchString("!=") or matchString("<") or 
          matchString("<=") or matchString(">") or matchString(">=") do
        saveop = matched
        good, newast = parse_arith()
        if not good then return false, nil end
        ast = {{BIN_OP, saveop}, ast, newast}
    end 
    return true, ast
end

-- 13. Arithmetic Expression
function parse_arith()
    local good, ast, saveop, newast
    good, ast = parse_term()
    if not good then return false, nil end

    while matchString("+") or matchString("-") do
        saveop = matched
        good, newast = parse_term()
        if not good then return false, nil end
        ast = {{BIN_OP, saveop}, ast, newast}
    end 
    return true, ast
end

-- 14. Term
function parse_term()
    local good, ast, saveop, newast
    good, ast = parse_factor()
    if not good then return false, nil end

    while matchString("*") or matchString("/") or matchString("%") do
        saveop = matched
        good, newast = parse_factor()
        if not good then return false, nil end
        ast = {{BIN_OP, saveop}, ast, newast}
    end 
    return true, ast
end

-- 15-20. Factors
function parse_factor()
    local good, ast, saveop, savelex, newast

    -- 15. Factor: Numeric Literal
    if matchCat(lexit.NUMLIT) then
        return true, {NUMLIT_VAL, matched}

    -- 16. Factor: Parenthesized Expression
    elseif matchString("(") then
        good, ast = parse_expr()
        if not good then return false, nil end
        if not matchString(")") then return false, nil end
        return true, ast

    -- 17. Factor: Unary Operator
    elseif matchString("+") or matchString("-") or matchString("!") then
        saveop = matched
        good, ast = parse_factor()
        if not good then return false, nil end
        return true, {{UN_OP, saveop}, ast}

    -- 18. Factor: Read Number
    elseif matchString("readnum") then
        if matchString("(") and matchString(")") then
            return true, {READNUM_CALL}
        end
        return false, nil

    -- 19. Factor: Random Number
    elseif matchString("rnd") and matchString("(") then
        good, ast = parse_expr()
        if not good then return false, nil end
        if not matchString(")") then return false, nil end
        return true, {RND_CALL, ast}

    -- 20. Factor: Beginning with Identifier
    elseif matchCat(lexit.ID) then
        savelex = matched
        if matchString("(") then
            if not matchString(")") then return false, nil end
            return true, {FUNC_CALL, savelex}
        elseif matchString("[") then
            good, ast = parse_expr()
            if not good then return false, nil end
            if not matchString("]") then return false, nil end
            return true, {ARRAY_VAR, savelex, ast}
        else
            return true, {SIMPLE_VAR, savelex}
        end
    end
    return false, nil
end


return parseit