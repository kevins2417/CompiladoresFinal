#include "parser.h"
#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_banner() {
    printf("=========================================\n");
    printf("      PARSER LL(1) PARA MINI-0\n");
    printf("  Análisis Sintáctico con Recuperación\n");
    printf("        de Múltiples Errores\n");
    printf("=========================================\n\n");
}

void print_error_summary(int error_count) {
    printf("\n");
    printf("=========================================\n");
    printf("           RESUMEN DE ANÁLISIS\n");
    printf("=========================================\n");
    
    if (error_count == 0) {
        printf("✅ ANÁLISIS EXITOSO\n");
        printf("   No se encontraron errores sintácticos\n");
    } else {
        printf("❌ SE ENCONTRARON ERRORES\n");
        printf("   Total de errores reportados: %d\n", error_count);
        
        if (error_count >= 10) {
            printf("   ⚠️  Muchos errores detectados\n");
            printf("   Revise la estructura completa del programa\n");
        } else if (error_count >= 5) {
            printf("   🔍 Varios errores - revise la sintaxis\n");
        } else {
            printf("   🛠️  Pocos errores - fácil de corregir\n");
        }
    }
    printf("=========================================\n");
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo.mini0>\n", argv[0]);
        fprintf(stderr, "Ejemplo: %s programa.mini0\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "❌ Error: No se pudo abrir el archivo '%s'\n", argv[1]);
        fprintf(stderr, "   Verifique que el archivo existe y es legible\n");
        return 1;
    }

    print_banner();
    printf("📁 Archivo: %s\n", argv[1]);
    printf("⏰ Iniciando análisis...\n");
    printf("   El parser continuará después de cada error\n");
    printf("   y reportará todos los problemas encontrados\n\n");

    clock_t start = clock();
    
    init_parser(input);
    
    // Opción: usar análisis LL(1) o recursivo descendente
    printf("🎯 Modo: Parser Recursivo Descendente con Recuperación de Errores\n\n");
    parse_program();
    
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    int errors = get_error_count();
    
    print_error_summary(errors);
    printf("   ⏱️  Tiempo de análisis: %.3f segundos\n", cpu_time_used);
    printf("=========================================\n\n");

    fclose(input);
    
    if (errors > 0) {
        printf("💡 Sugerencia: Revise los errores arriba y corrija el código\n");
    } else {
        printf("🎉 ¡Programa sintácticamente correcto! Puede proceder a la siguiente fase\n");
    }
    
    return (errors == 0) ? 0 : 1;
}