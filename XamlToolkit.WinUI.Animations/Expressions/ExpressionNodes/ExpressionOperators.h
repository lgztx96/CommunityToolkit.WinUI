#pragma once

#include "BooleanNode.h"
#include "../ExpressionFunctions.h"
#include "Matrix3x2Node.h"
#include "Matrix4x4Node.h"
#include "QuaternionNode.h"
#include "ScalarNode.h"
#include "Vector2Node.h"
#include "Vector3Node.h"
#include "Vector4Node.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
    /// <summary>
/// Implements the == operator.
/// </summary>
/// <param name="left">The left.</param>
/// <param name="right">The right.</param>
/// <returns>The result of the operator.</returns>
    inline BooleanNode operator ==(BooleanNode left, BooleanNode right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Equals, left, right);
    }

    /// <summary>
    /// Implements the != operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator !=(BooleanNode left, BooleanNode right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::NotEquals, left, right);
    }

    /// <summary>
    /// Implements the &amp; operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator &(BooleanNode left, BooleanNode right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::And, left, right);
    }

    /// <summary>
    /// Implements the | operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator |(BooleanNode left, BooleanNode right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Or, left, right);
    }

    /// <summary>
    /// Implements the ! operator.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator !(BooleanNode value)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Not, value);
    }


    /// <summary>
    /// Implements the == operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator ==(const ColorNode& left, const ColorNode& right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Equals, left, right);
    }

    /// <summary>
    /// Implements the != operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator !=(const ColorNode& left, const ColorNode& right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::NotEquals, left, right);
    }


    /// <summary>
       /// Implements the + operator.
       /// </summary>
       /// <param name="left">The left.</param>
       /// <param name="right">The right.</param>
       /// <returns>The result of the operator.</returns>
    inline Matrix3x2Node operator +(const Matrix3x2Node& left, const Matrix3x2Node& right)
    {
        return ExpressionFunctions::Function<Matrix3x2Node>(ExpressionNodeType::Add, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Matrix3x2Node operator -(const Matrix3x2Node& left, const Matrix3x2Node& right)
    {
        return ExpressionFunctions::Function<Matrix3x2Node>(ExpressionNodeType::Subtract, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns>The result of the operator.</returns>
    inline Matrix3x2Node operator -(const Matrix3x2Node& value)
    {
        return ExpressionFunctions::Function<Matrix3x2Node>(ExpressionNodeType::Negate, value);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Matrix3x2Node operator *(const Matrix3x2Node& left, const ScalarNode& right)
    {
        return ExpressionFunctions::Function<Matrix3x2Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Matrix3x2Node operator *(const Matrix3x2Node& left, const Matrix3x2Node& right)
    {
        return ExpressionFunctions::Function<Matrix3x2Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the == operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator ==(const Matrix3x2Node& left, const Matrix3x2Node& right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Equals, left, right);
    }

    /// <summary>
    /// Implements the != operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator !=(const Matrix3x2Node& left, const Matrix3x2Node& right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::NotEquals, left, right);
    }

    /// <summary>
        /// Implements the + operator.
        /// </summary>
        /// <param name="left">The left.</param>
        /// <param name="right">The right.</param>
        /// <returns>The result of the operator.</returns>
    inline Matrix4x4Node operator +(const Matrix4x4Node& left, const Matrix4x4Node& right)
    {
        return ExpressionFunctions::Function<Matrix4x4Node>(ExpressionNodeType::Add, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Matrix4x4Node operator -(const Matrix4x4Node& left, const Matrix4x4Node& right)
    {
        return ExpressionFunctions::Function<Matrix4x4Node>(ExpressionNodeType::Subtract, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns>The result of the operator.</returns>
    inline Matrix4x4Node operator -(const Matrix4x4Node& value)
    {
        return ExpressionFunctions::Function<Matrix4x4Node>(ExpressionNodeType::Negate, value);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Matrix4x4Node operator *(const Matrix4x4Node& left, const ScalarNode& right)
    {
        return ExpressionFunctions::Function<Matrix4x4Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Matrix4x4Node operator *(const Matrix4x4Node& left, const Matrix4x4Node& right)
    {
        return ExpressionFunctions::Function<Matrix4x4Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the == operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator ==(const Matrix4x4Node& left, const Matrix4x4Node& right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Equals, left, right);
    }

    /// <summary>
    /// Implements the != operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator !=(const Matrix4x4Node& left, const Matrix4x4Node& right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::NotEquals, left, right);
    }

    /// <summary>
        /// Implements the * operator.
        /// </summary>
        /// <param name="left">The left.</param>
        /// <param name="right">The right.</param>
        /// <returns>The result of the operator.</returns>
    inline QuaternionNode operator *(const QuaternionNode& left, const ScalarNode& right)
    {
        return ExpressionFunctions::Function<QuaternionNode>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline QuaternionNode operator *(const QuaternionNode& left, const QuaternionNode& right)
    {
        return ExpressionFunctions::Function<QuaternionNode>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the / operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline QuaternionNode operator /(const QuaternionNode& left, const QuaternionNode& right)
    {
        return ExpressionFunctions::Function<QuaternionNode>(ExpressionNodeType::Divide, left, right);
    }

    /// <summary>
    /// Implements the == operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator ==(const QuaternionNode& left, const QuaternionNode& right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Equals, left, right);
    }

    /// <summary>
    /// Implements the != operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator !=(const QuaternionNode& left, const QuaternionNode& right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::NotEquals, left, right);
    }


    /// <summary>
        /// Implements the + operator.
        /// </summary>
        /// <param name="left">The left.</param>
        /// <param name="right">The right.</param>
        /// <returns>The result of the operator.</returns>
    inline ScalarNode operator +(const ScalarNode& left, const ScalarNode& right)
    {
        return ExpressionFunctions::Function<ScalarNode>(ExpressionNodeType::Add, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline ScalarNode operator -(const ScalarNode& left, const ScalarNode& right)
    {
        return ExpressionFunctions::Function<ScalarNode>(ExpressionNodeType::Subtract, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns>The result of the operator.</returns>
    inline ScalarNode operator -(const ScalarNode& value)
    {
        return ExpressionFunctions::Function<ScalarNode>(ExpressionNodeType::Negate, value);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline ScalarNode operator *(const ScalarNode& left, const ScalarNode& right)
    {
        return ExpressionFunctions::Function<ScalarNode>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Vector2Node operator *(const ScalarNode& left, const Vector2Node& right)
    {
        return ExpressionFunctions::Function<Vector2Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Vector3Node operator *(const ScalarNode& left, const Vector3Node& right)
    {
        return ExpressionFunctions::Function<Vector3Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Vector4Node operator *(const ScalarNode& left, const Vector4Node& right)
    {
        return ExpressionFunctions::Function<Vector4Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Matrix4x4Node operator *(const ScalarNode& left, const Matrix4x4Node& right)
    {
        return ExpressionFunctions::Function<Matrix4x4Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the / operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline ScalarNode operator /(const ScalarNode& left, const ScalarNode& right)
    {
        return ExpressionFunctions::Function<ScalarNode>(ExpressionNodeType::Divide, left, right);
    }

    /// <summary>
    /// Implements the % operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline ScalarNode operator %(const ScalarNode& left, const ScalarNode& right)
    {
        return ExpressionFunctions::Function<ScalarNode>(ExpressionNodeType::Modulus, left, right);
    }

    /// <summary>
    /// Implements the == operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator ==(const ScalarNode& left, const ScalarNode& right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Equals, left, right);
    }

    /// <summary>
    /// Implements the != operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator !=(ScalarNode left, ScalarNode right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::NotEquals, left, right);
    }

    /// <summary>
    /// Implements the &lt;= operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator <=(ScalarNode left, ScalarNode right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::LessThanEquals, left, right);
    }

    /// <summary>
    /// Implements the &lt; operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator <(ScalarNode left, ScalarNode right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::LessThan, left, right);
    }

    /// <summary>
    /// Implements the &gt;= operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator >=(ScalarNode left, ScalarNode right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::GreaterThanEquals, left, right);
    }

    /// <summary>
    /// Implements the &gt; operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator >(ScalarNode left, ScalarNode right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::GreaterThan, left, right);
    }

    /// <summary>
        /// Implements the + operator.
        /// </summary>
        /// <param name="left">The left.</param>
        /// <param name="right">The right.</param>
        /// <returns>The result of the operator.</returns>
    Vector2Node operator +(Vector2Node left, Vector2Node right)
    {
        return ExpressionFunctions::Function<Vector2Node>(ExpressionNodeType::Add, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    Vector2Node operator -(Vector2Node left, Vector2Node right)
    {
        return ExpressionFunctions::Function<Vector2Node>(ExpressionNodeType::Subtract, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns>The result of the operator.</returns>
    Vector2Node operator -(Vector2Node value)
    {
        return ExpressionFunctions::Function<Vector2Node>(ExpressionNodeType::Negate, value);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    Vector2Node operator *(Vector2Node left, ScalarNode right)
    {
        return ExpressionFunctions::Function<Vector2Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    Vector2Node operator *(Vector2Node left, Vector2Node right)
    {
        return ExpressionFunctions::Function<Vector2Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the / operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    Vector2Node operator /(Vector2Node left, Vector2Node right)
    {
        return ExpressionFunctions::Function<Vector2Node>(ExpressionNodeType::Divide, left, right);
    }

    /// <summary>
    /// Implements the % operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    Vector2Node operator %(Vector2Node left, Vector2Node right)
    {
        return ExpressionFunctions::Function<Vector2Node>(ExpressionNodeType::Modulus, left, right);
    }

    /// <summary>
    /// Implements the == operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    BooleanNode operator ==(Vector2Node left, Vector2Node right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Equals, left, right);
    }

    /// <summary>
    /// Implements the != operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    BooleanNode operator !=(Vector2Node left, Vector2Node right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::NotEquals, left, right);
    }


    /// <summary>
        /// Implements the + operator.
        /// </summary>
        /// <param name="left">The left.</param>
        /// <param name="right">The right.</param>
        /// <returns>The result of the operator.</returns>
    Vector3Node operator +(Vector3Node left, Vector3Node right)
    {
        return ExpressionFunctions::Function<Vector3Node>(ExpressionNodeType::Add, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    Vector3Node operator -(Vector3Node left, Vector3Node right)
    {
        return ExpressionFunctions::Function<Vector3Node>(ExpressionNodeType::Subtract, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns>The result of the operator.</returns>
    Vector3Node operator -(Vector3Node value)
    {
        return ExpressionFunctions::Function<Vector3Node>(ExpressionNodeType::Negate, value);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    Vector3Node operator *(Vector3Node left, ScalarNode right)
    {
        return ExpressionFunctions::Function<Vector3Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    Vector3Node operator *(Vector3Node left, Vector3Node right)
    {
        return ExpressionFunctions::Function<Vector3Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the / operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    Vector3Node operator /(Vector3Node left, Vector3Node right)
    {
        return ExpressionFunctions::Function<Vector3Node>(ExpressionNodeType::Divide, left, right);
    }

    /// <summary>
    /// Implements the % operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    Vector3Node operator %(Vector3Node left, Vector3Node right)
    {
        return ExpressionFunctions::Function<Vector3Node>(ExpressionNodeType::Modulus, left, right);
    }

    /// <summary>
    /// Implements the == operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    BooleanNode operator ==(Vector3Node left, Vector3Node right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Equals, left, right);
    }

    /// <summary>
    /// Implements the != operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    BooleanNode operator !=(Vector3Node left, Vector3Node right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::NotEquals, left, right);
    }


    /// <summary>
        /// Implements the + operator.
        /// </summary>
        /// <param name="left">The left.</param>
        /// <param name="right">The right.</param>
        /// <returns>The result of the operator.</returns>
    inline Vector4Node operator +(const Vector4Node& left, const Vector4Node& right)
    {
        return ExpressionFunctions::Function<Vector4Node>(ExpressionNodeType::Add, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Vector4Node operator -(Vector4Node left, Vector4Node right)
    {
        return ExpressionFunctions::Function<Vector4Node>(ExpressionNodeType::Subtract, left, right);
    }

    /// <summary>
    /// Implements the - operator.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns>The result of the operator.</returns>
    inline Vector4Node operator -(Vector4Node value)
    {
        return ExpressionFunctions::Function<Vector4Node>(ExpressionNodeType::Negate, value);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Vector4Node operator *(Vector4Node left, ScalarNode right)
    {
        return ExpressionFunctions::Function<Vector4Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the * operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Vector4Node operator *(Vector4Node left, Vector4Node right)
    {
        return ExpressionFunctions::Function<Vector4Node>(ExpressionNodeType::Multiply, left, right);
    }

    /// <summary>
    /// Implements the / operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Vector4Node operator /(Vector4Node left, Vector4Node right)
    {
        return ExpressionFunctions::Function<Vector4Node>(ExpressionNodeType::Divide, left, right);
    }

    /// <summary>
    /// Implements the % operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline Vector4Node operator %(Vector4Node left, Vector4Node right)
    {
        return ExpressionFunctions::Function<Vector4Node>(ExpressionNodeType::Modulus, left, right);
    }

    /// <summary>
    /// Implements the == operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator ==(Vector4Node left, Vector4Node right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::Equals, left, right);
    }

    /// <summary>
    /// Implements the != operator.
    /// </summary>
    /// <param name="left">The left.</param>
    /// <param name="right">The right.</param>
    /// <returns>The result of the operator.</returns>
    inline BooleanNode operator !=(Vector4Node left, Vector4Node right)
    {
        return ExpressionFunctions::Function<BooleanNode>(ExpressionNodeType::NotEquals, left, right);
    }
}
