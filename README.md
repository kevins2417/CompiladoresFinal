# Parser LL(1) para Mini-0

## Descripción
Parser recursivo descendente LL(1) para el lenguaje Mini-0 con manejo robusto de múltiples errores.

## Características Principales

### ✅ Implementado
- **Parser Recursivo Descendente** LL(1)
- **Manejo de Múltiples Errores** (no se detiene en el primer error)
- **Recuperación de Errores** inteligente
- **Tabla LL(1)** para análisis predictivo
- **Reportes Detallados** con ubicación exacta

### 🎯 Gramática Transformada
La gramática original fue transformada para:
- Eliminar recursión por izquierda
- Aplicar factorización por izquierda  
- Cumplir condiciones LL(1)

## 🚀 Compilación

### Compilación Automática
```bash
make valid1.mini0
make multiple_errors.mini0
