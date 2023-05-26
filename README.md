# CS-Lexer


A simple lexer for C# code that highlights different language elements such as operators, keywords, comments, strings, and literals.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [License](#license)

## Introduction

The C# Lexer is a utility that parses C# source code and adds HTML tags to highlight different language elements. It helps improve code readability and provides a visual representation of the code structure.

## Features

- Highlights operators, keywords, comments, strings, and literals.
- Supports single-line and multiline comments.
- Handles various types of strings, including escape sequences.
- Differentiates between different types of literals, such as binary and hexadecimal.

## Getting Started

### Prerequisites

- C++ compiler that supports C++17 or later.
- Standard C++ libraries.

### Installation

1. Clone the repository: `git clone https://github.com/DiegoSandovalV/CS-Lexer.git`
2. Navigate to the project directory: `cd src`
3. Build the project: `g++ main.cpp -o lexer`

## Usage

1. Place your C# code files in the `csharp_examples` directory.
2. Run the lexer: `./lexer`
3. The processed HTML files will be generated in the `output` directory.
4. Open the generated HTML files in a web browser to view the syntax-highlighted code.


## License

This project is licensed under the [MIT License](LICENSE).
