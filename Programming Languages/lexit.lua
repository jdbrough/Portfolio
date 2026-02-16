--lexit.lua 
--Jonathan Brough
--02/17/25

local lexit = {}

lexit.KEY    = 1
lexit.ID     = 2
lexit.NUMLIT = 3
lexit.STRLIT = 4
lexit.OP     = 5
lexit.PUNCT  = 6
lexit.MAL    = 7

lexit.catnames = {
    "Keyword",
    "Identifier",
    "NumericLiteral",
    "StringLiteral",
    "Operator",
    "Punctuation",
    "Malformed"
}

local function isLetter(c)
    if c:len() ~= 1 then
        return false
    elseif c >= "A" and c <= "Z" then
        return true
    elseif c >= "a" and c <= "z" then
        return true
    else
        return false
    end
end

local function isDigit(c)
    if c:len() ~= 1 then
        return false
    elseif c >= "0" and c <= "9" then
        return true
    else
        return false
    end
end

local function isWhitespace(c)
    if c:len() ~= 1 then
        return false
    elseif c == " " or c == "\t" or c == "\n" or c == "\r"
      or c == "\f" then
        return true
    else
        return false
    end
end

local function isPrintableASCII(c)
    if c:len() ~= 1 then
        return false
    elseif c >= " " and c <= "~" then
        return true
    else
        return false
    end
end

local function isIllegal(c)
    if c:len() ~= 1 then
        return false
    elseif isWhitespace(c) then
        return false
    elseif isPrintableASCII(c) then
        return false
    else
        return true
    end
end

function lexit.lex(program)

    local pos
    local state
    local ch        
    local lexstr
    local category
    local handlers
    local quote
    local keynames = {
        "chr",
        "elif",
        "else",
        "end",
        "func",
        "if",
        "print",
        "println",
        "readnum",
        "return",
        "rnd",
        "while"
    }

    local DONE   = 0
    local START  = 1
    local LETTER = 2
    local DIGIT  = 3
    local QUOTE  = 4
    local EQUAL  = 5
    local EXP    = 6


    local function currChar()
        return program:sub(pos, pos)
    end

    local function nextChar()
        return program:sub(pos+1, pos+1)
    end

    local function nextNextChar()
        return program:sub(pos+2, pos+2)
    end

    local function drop1()
        pos = pos+1
    end

    local function add1()
        lexstr = lexstr .. currChar()
        drop1()
    end

    local function add2()
        add1()
        add1()
    end

    local function skipToNextLexeme()
        while true do
            -- Skip whitespace characters
            while isWhitespace(currChar()) do
                drop1()
            end

            -- Done if no comment
            if currChar() ~= "#" then
                break
            end

            -- Skip comment
            drop1()  -- Drop "#"
            while true do
                if currChar() == "\n" then
                    drop1()  -- Drop "\n"
                    break
                elseif currChar() == "" then  -- End of input?
                   return
                end
                drop1()  -- Drop character inside comment
            end
        end
    end

    -- State DONE: don't run
    local function handle_DONE()
        error("'DONE' state should not be handled\n")
    end

    -- State START: start of lexeme
    local function handle_START()
        if isIllegal(ch) then
            add1()
            state = DONE
            category = lexit.MAL
        elseif isLetter(ch) or ch == "_" then
            add1()
            state = LETTER
        elseif isDigit(ch) then
            add1()
            state = DIGIT
        elseif ch == "'" or ch == '"' then
            quote = ch
            add1()
            state = QUOTE
        elseif ch == "!" or ch == "=" or ch == "<" or ch == ">" then
            add1()
            state = EQUAL
        elseif ch == "+" or ch == "-" or ch == "*" or ch == "/" or 
                ch == "%" or ch == "[" or ch == "]" then
            add1()
            state = DONE
            category = lexit.OP
        elseif (ch == "|" and nextChar() == "|") or 
                (ch == "&" and nextChar() == "&") then
            add2()
            state = DONE
            category = lexit.OP
        else
            add1()
            state = DONE
            category = lexit.PUNCT
        end
    end

    -- State LETTER: ID or KEY
    local function handle_LETTER()
        if isLetter(ch) or isDigit(ch) or ch == "_" then
            add1()
        else
            state = DONE
            category = lexit.ID
            for k,v in ipairs(keynames) do
                if lexstr == v then
                    category = lexit.KEY
                end
            end
        end
    end

    -- State DIGIT: NUMLIT
    local function handle_DIGIT()
        if isDigit(ch) then
            add1()
        elseif ch == "e" or ch == "E" then
            if isDigit(nextChar()) then
                add1()
                state = EXP
            elseif nextChar() == "+" and isDigit(nextNextChar()) then
                add2()
                state = EXP
            else 
                state = DONE
                category = lexit.NUMLIT
            end 
        else
            state = DONE
            category = lexit.NUMLIT
        end
    end

    -- State QUOTE: STRLIT starting 
    local function handle_QUOTE()
        if ch == quote then
            add1()
            state = DONE
            category = lexit.STRLIT
        elseif ch == "\n" or ch == "" then
            state = DONE
            category = lexit.MAL
        else
            add1()
        end
    end

    -- State EQUAL: OP that can slap on an =
    local function handle_EQUAL()  
        if ch == "=" then
            add1()
            state = DONE
            category = lexit.OP
        else
            state = DONE
            category = lexit.OP
        end
    end

    -- State EXP: exponent of NUMLIT
    local function handle_EXP()
        if isDigit(ch) then
            add1()
        else 
            state = DONE
            category = lexit.NUMLIT
        end
    end

    handlers = {
        [DONE]=handle_DONE,
        [START]=handle_START,
        [LETTER]=handle_LETTER,
        [DIGIT]=handle_DIGIT,
        [QUOTE]=handle_QUOTE,
        [EQUAL]=handle_EQUAL,
        [EXP]=handle_EXP
    }

    --iterator func
    local function getLexeme()
        if pos > program:len() then
            return nil, nil
        end
        lexstr = ""
        state = START
        while state ~= DONE do
            ch = currChar()
            handlers[state]()
        end

        skipToNextLexeme()
        return lexstr, category
    end

    pos = 1
    skipToNextLexeme()
    return getLexeme
end

return lexit


