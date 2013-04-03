local modname = 'luakit'
local M = {}
_G[modname] = M
package.loaded[modname] = M

-- 获取文件内容
function file_get_contents(filename)
	local file, tmp, err
	file, err = io.open(filename,'rb');
	if not file
	then
		return 0, err
	end
	tmp = file:read('*a')
	file:close()
	return tmp
end
-- 一次写入文件
function file_put_contents(filename, contents)
	local file, ret, err
	file, err = io.open(filename,'wb');
	if not file
	then
		return 0, err
	end
	ret, err = file:write(contents)
	file:close()
	if not ret
	then
		return nil, err
	else
		return #contents
	end
end
-- 打印table所有属性
function printr(...)

	local pred = {} -- 记录已经打印的table
	local tabnum = 1
	local indent = 0
	local indenter = "    "

	local function pr(tab)
		for i,v in pairs(tab)
		do
			print(indenter:rep(indent)..i,'=',v)

			if type(v) == 'table'
			then
				if pred[v] == nil
				then
					pred[v] = tabnum
					tabnum  = tabnum + 1
					print(indenter:rep(indent)..'{table #'..pred[v],' }')
					indent = indent + 1
					pr(v)
					indent = indent - 1
				else
					print(indenter:rep(indent)..'{table ref #',pred[v],' }')
				end
			end
		end -- for
	end

	for i, arg in ipairs{...} do
		if type(arg) ~= "table"
		then
			print(arg)
		else
			pr(arg)
		end
	end

end
-- string
local function stringindex(str,key)
	if type(key) == "number"
	then
		return str:byte(key)
	end
end
function string.assequence(str)
end
-- table 便捷函数
function table.push(list, ...)
	for i, v in ipairs{...} do
		table.insert(list, v)
	end
end
function table.pop(list)
	return table.remove(list,#list)
end
function table.unshift(list, ...)
	for i, v in ipairs{...} do
		table.insert(list, 1, v)
	end
end
function table.shift(list)
	return table.remove(list,1)
end
function table.asobject(tab)
	setmetatable(tab, {__index = table})
	return tab
end
