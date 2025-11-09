# Versión mínima de CMake
cmake_minimum_required(VERSION 4.0)

# Define el nombre del proyecto y el lenguaje
# Lo llamo UOC20251 como tu proyecto original de CodeLite [cite: 1]
project(UOC20251 C)

# Establece el estándar de C
set(CMAKE_C_STANDARD 17)

#    Crea un EJECUTABLE, no una biblioteca
add_executable(UOC20251

        # El punto de entrada principal
        src/main.c

        # Todos los demás archivos .c que necesita
        library.c
        NetUOC/src/api.c
        NetUOC/src/company.c
        NetUOC/src/csv.c
        NetUOC/src/date.c
        NetUOC/src/job.c
        NetUOC/src/person.c
        test/src/test.c
        test/src/test_pr1.c
        test/src/test_suite.c

        # Incluir los .h aquí no es estrictamente necesario
        # para compilar, pero ayuda a que CLion los
        # reconozca como parte del proyecto.
        library.h
        NetUOC/include/api.h
        NetUOC/include/company.h
        NetUOC/include/csv.h
        NetUOC/include/date.h
        NetUOC/include/error.h
        NetUOC/include/job.h
        NetUOC/include/person.h
        test/include/test.h
        test/include/test_data.h
        test/include/test_pr1.h
        test/include/test_suite.h
)

#    Añade las rutas donde están los .h
target_include_directories(UOC20251 PUBLIC

        # Para que encuentre library.h
        .

        # Para que encuentre csv.h, company.h, job.h, etc.
        NetUOC/include

        # Para que encuentre test_suite.h, test_pr1.h, etc.
        test/include
)