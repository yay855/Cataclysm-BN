export const isList = (line: string) => /^ *(-|\*) /.test(line)
export const isIndent = (line: string) => /^    /.test(line)
export const isCode = (line: string) => !isList(line) && isIndent(line)
export const isEmpty = (line: string) => line === "" || /^ *$/.test(line)
export const removeIndent = (line: string): string => line.replace(/^    /, "")
