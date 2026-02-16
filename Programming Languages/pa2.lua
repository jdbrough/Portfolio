-- pa2.lua
-- Jonathan Brough
-- 02/10/25
-- Lua module for Assignment 2

local pa2 = {}

-- Function: mapArray
function pa2.mapArray(f, t)
    local result = {}
    for k, v in ipairs(t) do
        result[k] = f(v)
    end
    return result
end

-- Function: mostCommon
function pa2.mostCommon(s)
    local count = {}
    local max_char, max_count, first_pos = "", 0, {}
    
    for i = 1, #s do
        local char = s:sub(i, i)
        count[char] = (count[char] or 0) + 1
        if not first_pos[char] then first_pos[char] = i end
        
        if count[char] > max_count or (count[char] == max_count and first_pos[char] < first_pos[max_char]) then
            max_char, max_count = char, count[char]
        end
    end
    
    return max_char
end

-- Coroutine: prefixSuffix
function pa2.prefixSuffix(s)
    
    for i = 0, s:len() do
        coroutine.yield(s:sub(0,i))
    end
    for i = 2, s:len() do
        coroutine.yield(s:sub(i,s:len()))
    end
end

-- Iterator: collatz
function pa2.collatz(n)
    return coroutine.wrap(function()
        local current = n
        while true do
            coroutine.yield(current)
            if current == 1 then
                break
            elseif current % 2 == 0 then
                current = current // 2
            else
                current = 3 * current + 1
            end
        end
    end)
end

return pa2
