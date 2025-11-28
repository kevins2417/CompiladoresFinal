# 🎓 Parser LL(1) para el Lenguaje Mini-0

<div align="center">

![Parser Status](https://img.shields.io/badge/status-active-success.svg)
![Language](https://img.shields.io/badge/language-C-blue.svg)
![Flex](https://img.shields.io/badge/lexer-Flex-orange.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

**Analizador Sintáctico LL(1) con Recuperación de Múltiples Errores**

</div>

---

## 👥 Autores

| Nombre | Código | Email |
|--------|--------|-------|
| **Edwin Eduardo Ccama Pari** | 2025-777 | eccamap@ulasalle.edu.pe |
| **Kevin Alexis Chambilla Gonzales** | 2025-666 | kchambillag@ulasalle.edu.pe |

---

## 🏛️ Información Académica

- **Universidad:** Universidad Nacional La Salle
- **Facultad:** Facultad de Ingeniería y Arquitectura - Ing. Software
- **Curso:** Compiladores
- **Profesora:** Dra. Marcela Quispe Cruz
- **Ciclo Académico:** 2025-I
- **Fecha:** Noviembre 2025

---

## 📖 Descripción del Proyecto

Este proyecto implementa un **analizador sintáctico LL(1) recursivo descendente** para el lenguaje de programación educativo **Mini-0**. El parser es capaz de:

- ✅ Analizar la estructura sintáctica de programas Mini-0
- ✅ Detectar y reportar múltiples errores sintácticos
- ✅ Recuperarse de errores y continuar el análisis
- ✅ Generar reportes detallados con ubicación exacta de errores
- ✅ Mostrar la tabla de análisis predictivo LL(1)

### 🎯 Características Principales

- **Parser Recursivo Descendente:** Implementación directa de la gramática
- **Tabla LL(1):** Análisis predictivo con un token de lookahead
- **Manejo Robusto de Errores:** No se detiene en el primer error
- **Recuperación Inteligente:** Busca puntos de sincronización
- **Reportes Detallados:** Línea, columna y contexto de cada error

---

## 🗂️ Estructura del Proyecto
```
parser_analizadorsintactico/
├── src/
│   ├── lexer.l              # Analizador léxico (Flex)
│   ├── parser.c             # Analizador sintáctico
│   ├── parser.h             # Cabeceras del parser
│   ├── ll1.c                # Tabla LL(1)
│   ├── tokens.c             # Definiciones de tokens
│   ├── tokens.h             # Cabeceras de tokens
│   └── main.c               # Programa principal
├── tests/
│   ├── test_correcto.mini0  # Programa sin errores
│   ├── test_errores.mini0   # Programa con errores
│   ├── valid1.mini0         # Test válido 1
│   └── multiple_errors.mini0 # Test múltiples errores
├── docs/
│   └── gramatica.md         # Documentación de la gramática
├── Makefile                 # Script de compilación
├── README.md                # Este archivo
└── .gitignore              # Archivos ignorados por Git
```

---

## 📐 Gramática del Lenguaje Mini-0

### Gramática Original (Ambigua)
```
program         → PROGRAM IDENTIFIER ; declarations compound_stmt .

declarations    → VAR declaration_list | ε

declaration_list → declaration declaration_list | declaration

declaration     → id_list : type ;

id_list         → IDENTIFIER | IDENTIFIER , id_list

type            → INTEGER | BOOLEAN

compound_stmt   → BEGIN stmt_list END

stmt_list       → stmt | stmt ; stmt_list

stmt            → assignment_stmt
                | compound_stmt
                | if_stmt
                | while_stmt
                | read_stmt
                | write_stmt

assignment_stmt → IDENTIFIER := expression

if_stmt         → IF expression THEN stmt [ELSE stmt]

while_stmt      → WHILE expression DO stmt

read_stmt       → READ ( id_list )

write_stmt      → WRITE ( expression )

expression      → simple_expr | simple_expr relop simple_expr

simple_expr     → term | simple_expr addop term

term            → factor | term mulop factor

factor          → IDENTIFIER
                | NUMBER
                | TRUE
                | FALSE
                | ( expression )
                | NOT factor

relop           → < | <= | > | >= | = | <>

addop           → + | - | OR

mulop           → * | / | AND
```

### Gramática Transformada LL(1)

Para eliminar recursión por izquierda y ambigüedad:
```
program         → PROGRAM IDENTIFIER ; declarations compound_stmt .

declarations    → VAR declaration_list | ε

declaration_list → declaration declaration_list'
declaration_list' → declaration declaration_list' | ε

declaration     → id_list : type ;

id_list         → IDENTIFIER id_list'
id_list'        → , IDENTIFIER id_list' | ε

type            → INTEGER | BOOLEAN

compound_stmt   → BEGIN stmt_list END

stmt_list       → stmt stmt_list'
stmt_list'      → ; stmt stmt_list' | ε

stmt            → assignment_stmt
                | compound_stmt
                | if_stmt
                | while_stmt
                | read_stmt
                | write_stmt

assignment_stmt → IDENTIFIER := expression

if_stmt         → IF expression THEN stmt else_part
else_part       → ELSE stmt | ε

while_stmt      → WHILE expression DO stmt

read_stmt       → READ ( id_list )

write_stmt      → WRITE ( expression )

expression      → simple_expr expression'
expression'     → relop simple_expr | ε

simple_expr     → term simple_expr'
simple_expr'    → addop term simple_expr' | ε

term            → factor term'
term'           → mulop factor term' | ε

factor          → IDENTIFIER
                | NUMBER
                | TRUE
                | FALSE
                | ( expression )
                | NOT factor

relop           → < | <= | > | >= | = | <>
addop           → + | - | OR
mulop           → * | / | AND
```

---

## 📊 Tabla de Análisis Sintáctico LL(1)

La siguiente tabla muestra las producciones a aplicar según el no terminal en la pila y el token de entrada:

### Tabla LL(1) Completa

| No Terminal | program | var | begin | end | if | then | else | while | do | read | write | integer | boolean | identifier | ; | : | . | , | ( | ) | := | Otros |
|------------|---------|-----|-------|-----|----|----|------|-------|-------|------|-------|---------|---------|---------|---|---|---|---|---|---|---|-------|
| **program** | 1 | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - |
| **declarations** | - | 2 | ε | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - |
| **declaration_list** | - | - | - | - | - | - | - | - | - | - | - | - | - | 3 | - | - | - | - | - | - | - | - |
| **declaration** | - | - | - | - | - | - | - | - | - | - | - | - | - | 4 | - | - | - | - | - | - | - | - |
| **id_list** | - | - | - | - | - | - | - | - | - | - | - | - | - | 5 | - | - | - | - | - | - | - | - |
| **id_list'** | - | - | - | - | - | - | - | - | - | - | - | - | - | - | ε | ε | - | 6 | - | ε | - | - |
| **type** | - | - | - | - | - | - | - | - | - | - | - | 7 | 8 | - | - | - | - | - | - | - | - | - |
| **compound_stmt** | - | - | 9 | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - |
| **stmt_list** | - | - | 10 | - | 10 | - | - | 10 | - | 10 | 10 | - | - | 10 | - | - | - | - | - | - | - | - |
| **stmt_list'** | - | - | - | ε | - | - | ε | - | - | - | - | - | - | - | 11 | - | - | - | - | - | - | - |
| **stmt** | - | - | 13 | - | 14 | - | - | 15 | - | 16 | 17 | - | - | 12 | - | - | - | - | - | - | - | - |
| **assignment_stmt** | - | - | - | - | - | - | - | - | - | - | - | - | - | 18 | - | - | - | - | - | - | - | - |
| **if_stmt** | - | - | - | - | 19 | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - |
| **else_part** | - | - | - | ε | - | - | 20 | - | - | - | - | - | - | - | ε | - | - | - | - | - | - | - |
| **while_stmt** | - | - | - | - | - | - | - | 21 | - | - | - | - | - | - | - | - | - | - | - | - | - | - |
| **read_stmt** | - | - | - | - | - | - | - | - | - | 22 | - | - | - | - | - | - | - | - | - | - | - | - |
| **write_stmt** | - | - | - | - | - | - | - | - | - | - | 23 | - | - | - | - | - | - | - | - | - | - | - |
| **expression** | - | - | - | - | - | - | - | - | - | - | - | - | - | 24 | - | - | - | - | 24 | - | - | 24 |
| **expression'** | - | - | - | ε | - | ε | ε | - | ε | - | - | - | - | - | ε | - | - | - | - | ε | - | 25 |
| **simple_expr** | - | - | - | - | - | - | - | - | - | - | - | - | - | 26 | - | - | - | - | 26 | - | - | 26 |
| **simple_expr'** | - | - | - | ε | - | ε | ε | - | ε | - | - | - | - | - | ε | - | - | - | - | ε | - | 27 |
| **term** | - | - | - | - | - | - | - | - | - | - | - | - | - | 28 | - | - | - | - | 28 | - | - | 28 |
| **term'** | - | - | - | ε | - | ε | ε | - | ε | - | - | - | - | - | ε | - | - | - | - | ε | - | 29 |
| **factor** | - | - | - | - | - | - | - | - | - | - | - | - | - | 30 | - | - | - | - | 31 | - | - | 32 |

### Leyenda de Producciones
```
1.  program → PROGRAM IDENTIFIER ; declarations compound_stmt .
2.  declarations → VAR declaration_list
3.  declaration_list → declaration declaration_list'
4.  declaration → id_list : type ;
5.  id_list → IDENTIFIER id_list'
6.  id_list' → , IDENTIFIER id_list'
7.  type → INTEGER
8.  type → BOOLEAN
9.  compound_stmt → BEGIN stmt_list END
10. stmt_list → stmt stmt_list'
11. stmt_list' → ; stmt stmt_list'
12. stmt → assignment_stmt
13. stmt → compound_stmt
14. stmt → if_stmt
15. stmt → while_stmt
16. stmt → read_stmt
17. stmt → write_stmt
18. assignment_stmt → IDENTIFIER := expression
19. if_stmt → IF expression THEN stmt else_part
20. else_part → ELSE stmt
21. while_stmt → WHILE expression DO stmt
22. read_stmt → READ ( id_list )
23. write_stmt → WRITE ( expression )
24. expression → simple_expr expression'
25. expression' → relop simple_expr
26. simple_expr → term simple_expr'
27. simple_expr' → addop term simple_expr'
28. term → factor term'
29. term' → mulop factor term'
30. factor → IDENTIFIER
31. factor → ( expression )
32. factor → NOT factor
33. factor → NUMBER | TRUE | FALSE

ε = producción vacía (epsilon)
- = error sintáctico
```

### Conjuntos First y Follow

#### Conjuntos FIRST
```
FIRST(program)         = {PROGRAM}
FIRST(declarations)    = {VAR, ε}
FIRST(declaration)     = {IDENTIFIER}
FIRST(type)            = {INTEGER, BOOLEAN}
FIRST(compound_stmt)   = {BEGIN}
FIRST(stmt)            = {IDENTIFIER, BEGIN, IF, WHILE, READ, WRITE}
FIRST(expression)      = {IDENTIFIER, NUMBER, TRUE, FALSE, (, NOT}
FIRST(factor)          = {IDENTIFIER, NUMBER, TRUE, FALSE, (, NOT}
```

#### Conjuntos FOLLOW
```
FOLLOW(program)        = {$}
FOLLOW(declarations)   = {BEGIN}
FOLLOW(declaration)    = {IDENTIFIER, BEGIN}
FOLLOW(type)           = {;}
FOLLOW(stmt)           = {;, END, ELSE}
FOLLOW(expression)     = {THEN, DO, ), ;, END, ELSE}
FOLLOW(factor)         = {*, /, AND, +, -, OR, <, <=, >, >=, =, <>, THEN, DO, ), ;, END, ELSE}
```

---

## 🚀 Instalación y Uso

### Prerrequisitos
```bash
# Ubuntu/Debian/WSL
sudo apt-get update
sudo apt-get install flex gcc make

# Fedora/RHEL
sudo dnf install flex gcc make

# macOS
brew install flex gcc make
```

### Compilación
```bash
# Clonar el repositorio
git clone https://github.com/USUARIO/parser-mini0.git
cd parser-mini0

# Compilar el proyecto
make

# Limpiar archivos compilados
make clean
```

### Ejecución
```bash
# Ejecutar con archivo de prueba
./mini0parser tests/test_correcto.mini0

# Ejecutar con archivo con errores
./mini0parser tests/test_errores.mini0

# Atajo con Makefile
make test_correcto.mini0
make test_errores.mini0
```

---

## 📝 Ejemplos de Uso

### ✅ Programa Correcto

**Entrada:** `test_correcto.mini0`
```pascal
program Ejemplo;
var
    x, y: integer;
begin
    x := 5;
    y := x + 10;
    write(y)
end.
```

**Salida:**
```
=========================================
      PARSER LL(1) PARA MINI-0
=========================================

🏁 Iniciando análisis del programa...
   📝 Programa: Ejemplo
   📋 Procesando declaraciones VAR
     ├── Declaración
     │   ├── ID: x
     │   ├── ID: y
     │   └── Tipo: INTEGER
   🔨 Procesando bloque BEGIN-END
     ├── Sentencia: Asignación
     ├── Sentencia: Asignación
     ├── Sentencia: WRITE
   ✅ Bloque END correcto
   ✅ Punto final encontrado

=========================================
           RESUMEN DE ANÁLISIS
=========================================
✅ ANÁLISIS EXITOSO
   No se encontraron errores sintácticos
=========================================
```

### ❌ Programa con Errores

**Entrada:** `test_errores.mini0`

**Salida:**
```
❌ Error sintáctico en línea 1, columna 24:
   Esperaba: ';'
   Encontró: 'var' (VAR)
   ↳ Intentando recuperación...

❌ Error sintáctico en línea 3, columna 15:
   Esperaba: 'IDENTIFIER'
   Encontró: ':' (COLON)

❌ Error sintáctico en línea 7, columna 9:
   Esperaba: 'IDENTIFIER, NUMBER, TRUE, FALSE, '(' o NOT'
   Encontró: ';' (SEMICOLON)

=========================================
           RESUMEN DE ANÁLISIS
=========================================
❌ SE ENCONTRARON ERRORES
   Total de errores reportados: 8
   🔍 Varios errores - revise la sintaxis
=========================================
```

---

## 🧪 Tests Incluidos

| Archivo | Descripción | Resultado Esperado |
|---------|-------------|-------------------|
| `test_correcto.mini0` | Programa completo sin errores | ✅ Análisis exitoso |
| `test_errores.mini0` | Múltiples tipos de errores | ❌ 8+ errores detectados |
| `valid1.mini0` | Test de validación 1 | ✅ Análisis exitoso |
| `multiple_errors.mini0` | Recuperación de errores | ❌ Errores múltiples |

### Ejecutar todos los tests
```bash
# Test individual
make test_correcto.mini0

# O manualmente
./mini0parser tests/test_correcto.mini0
./mini0parser tests/test_errores.mini0
```

---

## 🛠️ Arquitectura del Sistema
```
┌─────────────────────────────────────────────────────────┐
│                    Archivo .mini0                        │
│              (Código fuente del programa)                │
└──────────────────┬──────────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────────┐
│              ANALIZADOR LÉXICO (Flex)                    │
│  ┌──────────────────────────────────────────────────┐   │
│  │  lexer.l → lex.yy.c                              │   │
│  │  • Reconoce tokens                               │   │
│  │  • Maneja comentarios                            │   │
│  │  • Reporta errores léxicos                       │   │
│  └──────────────────────────────────────────────────┘   │
└──────────────────┬──────────────────────────────────────┘
                   │ Stream de Tokens
                   ▼
┌─────────────────────────────────────────────────────────┐
│           ANALIZADOR SINTÁCTICO (Parser C)               │
│  ┌──────────────────────────────────────────────────┐   │
│  │  parser.c + ll1.c                                │   │
│  │  • Verifica estructura sintáctica                │   │
│  │  • Usa tabla LL(1)                               │   │
│  │  • Recuperación de errores                       │   │
│  │  • Parser recursivo descendente                  │   │
│  └──────────────────────────────────────────────────┘   │
└──────────────────┬──────────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────────┐
│                  REPORTE DE RESULTADOS                   │
│  • Árbol de derivación (implícito)                      │
│  • Lista de errores con ubicación                       │
│  • Estadísticas del análisis                            │
└─────────────────────────────────────────────────────────┘
```

---

## 📚 Referencias

1. **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Pearson Education.

2. **Cooper, K. D., & Torczon, L.** (2011). *Engineering a Compiler* (2nd ed.). Morgan Kaufmann.

3. **Appel, A. W.** (2004). *Modern Compiler Implementation in C*. Cambridge University Press.

4. **Fischer, C. N., Cytron, R. K., & LeBlanc, R. J.** (2009). *Crafting a Compiler*. Pearson.

5. **Grune, D., & Jacobs, C. J.** (2007). *Parsing Techniques: A Practical Guide* (2nd ed.). Springer.

---

## 🤝 Contribuciones

Las contribuciones son bienvenidas. Por favor:

1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add: AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

---

## 📄 Licencia

Este proyecto fue desarrollado con fines académicos para el curso de Compiladores de la UNMSM.
```
MIT License

Copyright (c) 2025 [Tu Nombre] y [Nombre Compañero]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 📧 Contacto

- **Autor 1:** Edwin Eduardo Ccama Pari - ECcamaP@ulasalle.edu.pe
- **Autor 2:** Kevin Alexis Chambilla Gonzales - KChambiallaG@ulasalle.edu.pe
- **Profesora:** Dra. Marcela Quispe Cruz - MQuispeCr@ulasalle.edu.pe

**Repositorio:** https://github.com/kevins2417/CompiladoresFinal

---

<div align="center">

**Universidad La Salle**

*Facultad de Ingenierías y Arquitectura*

Noviembre 2025

</div>
