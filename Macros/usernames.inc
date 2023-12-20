include assets.inc
getUserName macro mes1, mes2, name, pts, error
    showmes mes1
    getstr name
    checkNameChars name, error
    endl
    showmes mes2
    getstr pts
endm
