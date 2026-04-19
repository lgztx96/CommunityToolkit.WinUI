;; MIDL3 Highlight Queries

;; Keywords - Declaration types
[
  "import"
  "namespace"
  "runtimeclass"
  "interface"
  "enum"
  "struct"
  "delegate"
  "event"
  "attribute"
  "apicontract"
  "typedef"
] @keyword.type

;; Keywords - Modifiers
[
  "static"
  "unsealed"
  "sealed"
  "overridable"
  "protected"
] @keyword.modifier

;; Keywords - Other
[
  "requires"
  "out"
  "ref"
] @keyword

;; Builtin types (via grammar rule)
"void" @type.builtin
(builtin_type) @type.builtin

;; Property accessors
[
  "get"
  "set"
] @property.accessor

;; Boolean literals
[
  "true"
  "false"
] @boolean

;; Preprocessor directives
[
  "#include"
  "#define"
  "#undef"
  "#ifdef"
  "#ifndef"
  "#if"
  "#elif"
  "#else"
  "#endif"
  "#pragma"
] @keyword.directive

(preprocessor_value) @string

;; Type names (declarations)
(type_name) @type.definition
(attribute_type_name) @type.definition
(typedef_name) @type.definition

;; Type references
(qualified_type_identifier) @type
(qualified_identifier) @module

;; Namespace
(namespace_name) @module

;; Method names
(method_name) @function.method
(delegate_name) @function.method

;; Constructor names
(constructor_name) @constructor

;; Property names
(property_name) @property

;; Member names (struct fields, events)
(field_name) @variable.member
(event_name) @variable.member

;; Parameter names
(parameter_name) @variable

;; Enum member names
(enum_member_name) @constant

;; Return type
(return_type) @type

;; Modifier nodes
(modifier) @keyword.modifier
(parameter_modifier) @keyword.modifier

;; Attributes
(attribute_name) @attribute

;; Attribute arguments - identifiers are type references (e.g., contract name)
(attribute_argument
  (identifier) @type)

;; Literals
(string_literal) @string
(string_content) @string
(integer_literal) @number
(decimal_literal) @number
(hex_literal) @number
(float_literal) @number

;; Comments
(line_comment) @comment.line
(block_comment) @comment.block

;; Punctuation - Delimiters
[
  ";"
  ":"
  ","
  "."
] @punctuation.delimiter

;; Punctuation - Brackets
[
  "["
  "]"
  "{"
  "}"
  "("
  ")"
  "<"
  ">"
] @punctuation.bracket

;; Operators
"=" @operator

;; Include filename
(include_filename) @string