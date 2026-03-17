# Implementación del Algoritmo de Codificación Aritmética

## Descripción General

Este proyecto es una aplicación gráfica que implementa el algoritmo de codificación aritmética y proporciona una representación visual de los componentes utilizados en el proceso de codificación y decodificación. La aplicación permite al usuario ingresar texto, calcular la tabla de frecuencias, generar intervalos aritméticos para cada símbolo, mostrar el valor codificado y decodificado, y visualizar los subintervalos generados.

El objetivo del proyecto es tanto educativo como práctico. Demuestra cómo funciona internamente la compresión sin pérdida mientras proporciona una interfaz interactiva que facilita la comprensión del proceso. La interfaz gráfica hace más fácil entender cómo las frecuencias de los símbolos influyen en la construcción de intervalos y en los valores de codificación.

La aplicación está construida usando C++ y wxWidgets, con CMake utilizado como sistema de construcción. Separa la lógica central de codificación aritmética de la interfaz gráfica, siguiendo una arquitectura modular.

## Herramientas y Versiones

Este proyecto está escrito en C++20 y requiere un compilador C++ moderno con soporte completo para C++20. El compilador recomendado es Microsoft Visual C++ (MSVC) proporcionado con Visual Studio 2022.

La aplicación depende de wxWidgets 3.x. Fue construida y probada usando la configuración MSVC de 64 bits dinámica (vc_x64_dll) compilada en modo Release. wxWidgets debe ser compilada manualmente desde el código fuente antes de compilar este proyecto.

El sistema de construcción utilizado es CMake versión 3.20 o más reciente. CMake debe estar instalado y accesible desde la línea de comandos. Puede verificar esto ejecutando:

```bash
cmake --version
```

La configuración de desarrollo recomendada en Windows incluye Visual Studio 2022 con la carga de trabajo "Desarrollo de escritorio con C++" instalada. Durante la instalación, asegúrese de que las herramientas de construcción MSVC v143 y el SDK de Windows 10 o Windows 11 estén seleccionados. El proyecto utiliza la cadena de herramientas de 64 bits y debe ser compilado desde el "x64 Native Tools Command Prompt for VS" al compilar wxWidgets manualmente.

La aplicación fue desarrollada y probada en Windows 11 utilizando la cadena de herramientas MSVC x64. Aunque puede funcionar en Windows 10, no ha sido probada con MinGW u otros compiladores.

Antes de proceder con la configuración del entorno de desarrollo, asegúrese de que el siguiente software esté instalado y correctamente configurado en su sistema:

- Visual Studio 2022 con carga de trabajo C++
- CMake 3.20 o más reciente
- Código fuente de wxWidgets 3.x
- SDK de Windows 10 o 11
- Herramientas de construcción MSVC x64

Estos componentes son necesarios para compilar y ejecutar la aplicación según se describe en la siguiente sección.

## Configuración del Entorno de Desarrollo (Windows + MSVC)

Este proyecto fue desarrollado y probado en Windows usando Microsoft Visual Studio 2022 (MSVC x64), CMake y wxWidgets compiladas manualmente desde el código fuente.

Primero, descargue el código fuente de wxWidgets como un archivo ZIP de Windows desde:

<https://wxwidgets.org/downloads/>

Extraiga el archivo en:

```bash
C:\cpp-lib-wxwidgets
```

Luego cree una variable de entorno apuntando a este directorio. Abra PowerShell o Símbolo del sistema y ejecute:

```bash
setx WXWIN "C:\cpp-lib-wxwidgets"
```

Cierre y reabre la terminal después de establecer la variable.

Para compilar wxWidgets, abra el Símbolo del sistema del Desarrollador de Visual Studio:

```bash
x64 Native Tools Command Prompt for VS
```

Luego navegue al directorio de construcción MSVC de wxWidgets:

```bash
cd C:\cpp-lib-wxwidgets\build\msw
```

Compile wxWidgets en modo Release de 64 bits con bibliotecas compartidas:

```bash
nmake -f makefile.vc BUILD=release SHARED=1
```

Después de la compilación, las bibliotecas generadas aparecerán en:

```bash
C:\cpp-lib-wxwidgets\lib\vc_x64_dll
```

Como esta es una compilación compartida (DLL), los archivos DLL en tiempo de ejecución deben ser accesibles. Puede agregar uno u otro a su PATH del sistema:

```bash
setx PATH "%PATH%;C:\cpp-lib-wxwidgets\lib\vc_x64_dll"
```

O copiar los archivos DLL en el directorio del ejecutable de su proyecto después de compilar:

```bash
copy C:\cpp-lib-wxwidgets\lib\vc_x64_dll\*.dll build\Release\
```

A continuación, configure su CMakeLists.txt en la raíz de su proyecto de la siguiente manera:

```cmake
cmake_minimum_required(VERSION 3.15)
project(ArithmeticCoding)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(wxWidgets_ROOT_DIR $ENV{WXWIN})
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Core sources
set(CORE_SOURCES
    src/core/ArithmeticCoder.cpp
    src/core/Symbol.h
)

# UI sources
set(UI_SOURCES
    src/ui/MainFrame.cpp
    src/ui/MainFrame.h
)

# Main application source
set(MAIN_SOURCES
    src/main.cpp
)

set(ALL_SOURCES ${CORE_SOURCES} ${UI_SOURCES} ${MAIN_SOURCES})

add_executable(Arithmetic WIN32 ${ALL_SOURCES})

target_include_directories(Arithmetic PRIVATE src)
target_link_libraries(Arithmetic ${wxWidgets_LIBRARIES})
target_compile_definitions(Arithmetic PRIVATE UNICODE _UNICODE)
```

Para soporte de IntelliSense en Visual Studio Code, configure .vscode/c_cpp_properties.json de la siguiente manera:

```json
{
    "configurations": [
        {
            "name": "Win32",
            "compilerPath": "cl.exe",
            "intelliSenseMode": "windows-msvc-x64",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/cpp-lib-wxwidgets/include",
                "C:/cpp-lib-wxwidgets/lib/vc_x64_dll/mswu"
            ],
            "defines": [
                "NDEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "cppStandard": "c++20"
        }
    ],
    "version": 4
}
```

Finalmente, desde la raíz de su proyecto, configure y compile usando CMake:

```bash
cmake -S . -B build
cmake --build build --config Release
```

El ejecutable será generado en:

```bash
build\Release\Arithmetic.exe
```

Ejecute la aplicación usando:

```bash
build\Release\Arithmetic.exe
```

Si la aplicación no se inicia debido a DLL faltantes, verifique que las bibliotecas dinámicas de wxWidgets estén incluidas en el PATH del sistema o copiadas en el directorio Release.

Si necesita recompilar, use lo siguiente para una recompilación limpia:

```bash
rm -r -fo build
cmake -S . -B build
cmake --build build --config Release
```

## Compilación y Ejecución

Después de completar la configuración del entorno de desarrollo y compilar exitosamente wxWidgets en modo Release (vc_x64_dll), el proyecto puede ser compilado usando CMake y MSVC.

Abra una terminal en el directorio raíz del proyecto. Se recomienda usar PowerShell o Símbolo del sistema normal para configurar y compilar la aplicación, ya que wxWidgets ya ha sido compilada usando el x64 Native Tools Command Prompt.

Genere los archivos de construcción usando CMake con una compilación fuera de la fuente:

```bash
cmake -S . -B build
```

Este comando configura el proyecto y genera los archivos de construcción necesarios de Visual Studio dentro del directorio build.

A continuación, compile el proyecto en modo Release:

```bash
cmake --build build --config Release
```

Si la compilación se completa exitosamente, el ejecutable será generado en:

```bash
build\Release\Arithmetic.exe
```

Ejecute la aplicación usando:

```bash
build\Release\Arithmetic.exe
```

Si el enlazador reporta que no puede abrir el archivo ejecutable, asegúrese de que la aplicación no esté ya en ejecución y que ninguna instancia anterior bloquee el archivo.

Si la aplicación no se inicia debido a archivos DLL faltantes, verifique que las bibliotecas dinámicas de wxWidgets desde:

```bash
C:\cpp-lib-wxwidgets\lib\vc_x64_dll
```

estén incluidas en la variable de entorno PATH del sistema o copiadas directamente en el directorio build\Release.

Este proyecto utiliza una compilación compartida de wxWidgets (SHARED=1), por lo que los archivos DLL son necesarios en tiempo de ejecución.

## Estructura del Proyecto

El proyecto sigue una arquitectura modular que separa la lógica de codificación aritmética de la interfaz gráfica. El diseño de directorios está organizado de la siguiente manera:

```
ArithmeticCoding/
│
├── CMakeLists.txt
├── README.md
├── build/
│
├── src/
│   ├── main.cpp
│   │
│   ├── core/
│   │   ├── ArithmeticCoder.h
│   │   ├── ArithmeticCoder.cpp
│   │   ├── Symbol.h
│   │
│   └── ui/
│       ├── MainFrame.h
│       ├── MainFrame.cpp
│
└── .vscode/
    └── c_cpp_properties.json
```

El directorio raíz contiene el archivo CMakeLists.txt, que define la configuración de construcción, configuración del compilador, descubrimiento de fuentes e integración de wxWidgets.

El directorio src contiene el código fuente de la aplicación y se divide en componentes lógicos. La carpeta core implementa el algoritmo de codificación aritmética, incluyendo las estructuras de datos, construcción de intervalos, análisis de frecuencias y lógica de codificación y decodificación. Esta parte del proyecto es independiente de la interfaz gráfica y puede ser reutilizada en otros contextos.

La carpeta ui contiene todos los componentes de la interfaz de usuario construidos usando wxWidgets. MainFrame administra la ventana de la aplicación y el diseño, mostrando la tabla de frecuencias, los subintervalos generados, el valor codificado y el valor decodificado.

El archivo main.cpp sirve como punto de entrada de la aplicación e inicializa el objeto de aplicación wxWidgets.

El directorio build es generado automáticamente por CMake durante la compilación. Contiene archivos intermedios de construcción y el ejecutable final. Este directorio no es parte del código fuente y no debe ser confirmado en el control de versiones.

La carpeta .vscode contiene archivos de configuración para Visual Studio Code, incluyendo configuración de IntelliSense para wxWidgets y MSVC.

Esta separación asegura que el algoritmo de codificación aritmética permanezca independiente de la interfaz gráfica, haciendo que el proyecto sea más fácil de mantener, probar, depurar y extender en el futuro.

## Características Principales

### Tabla de Frecuencias

La aplicación calcula automáticamente la frecuencia de cada símbolo en la cadena de entrada y muestra una tabla con:
- **Símbolo (Si)**: El carácter en la cadena
- **Frecuencia (Fi)**: La cantidad de veces que aparece el símbolo
- **Intervalo (Ii)**: El intervalo inicial [bajo, alto) asignado al símbolo

### Subintervalos

Durante el proceso de codificación, se generan subintervalos para cada símbolo a medida que se procesa la cadena. Estos subintervalos se muestran con alta precisión (15 decimales) para permitir análisis detallado del proceso de codificación aritmética.

### Valor Codificado

Se selecciona un valor aleatorio dentro del intervalo final generado. Este valor representa la codificación comprimida de la cadena completa y se muestra junto con la cadena original.

### Decodificación

El algoritmo decodifica automáticamente el valor seleccionado comparándolo con los intervalos y reconstruyendo la cadena original. El resultado se muestra para verificación.

## Interfaz de Usuario

La interfaz está completamente en español e incluye:
- **Cadena de Entrada**: Campo de texto multilineal para ingresar el texto a codificar
- **Codificar y Decodificar**: Botón para iniciar el proceso
- **Limpiar**: Botón para limpiar todos los datos y volver a comenzar
- **Soporte UTF-8**: Todos los caracteres acentuados se muestran correctamente (ó, í, é, ñ, etc.)

## Requisitos del Sistema

- Windows 10 o 11
- Visual Studio 2022 con herramientas de compilación C++
- CMake 3.20 o más reciente
- wxWidgets 3.x compiladas en modo Release con MSVC x64

## Autor

Proyecto académico - Práctica de Codificación Aritmética
