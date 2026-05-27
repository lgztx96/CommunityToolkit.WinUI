#include "pch.h"
#include "winrt_module_imports.h"
#include "ExpressionFunctions.h"

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{

        ScalarNode ExpressionFunctions::ACos(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Acos, { val });
        }

        /// <summary>
        /// Returns the angle (in radians) whose sine is the specified number.
        /// </summary>
        /// <param name="val">Value between -1 and 1, for which to calculate the arcsine (the inverse sine).</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::ASin(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Asin, val);
        }

        /// <summary>
        /// Returns the angle (in radians) whose tangent is the specified number.
        /// </summary>
        /// <param name="val">Value for which to calculate the arctan (the inverse tan).</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::ATan(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Atan, val);
        }

        /// <summary>
        /// Returns the smallest integral value that is greater than or equal to the specified value.
        /// </summary>
        /// <param name="val">The floating point number to round.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Ceil(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Ceil, val);
        }

        /// <summary>
        /// Returns the cosine of the specified angle (in radians).
        /// </summary>
        /// <param name="val">An angle, measured in radians.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Cos(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Cos, val);
        }

        /// <summary>
        /// Returns the largest integer less than or equal to the specified value.
        /// </summary>
        /// <param name="val">The floating point number to round.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Floor(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Floor, val);
        }

        /// <summary>
        /// Returns the natural (base e) logarithm of a specified number.
        /// </summary>
        /// <param name="val">The number whose natural logarithm is to be returned.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Ln(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Ln, val);
        }

        /// <summary>
        /// Returns the base 10 logarithm of a specified number.
        /// </summary>
        /// <param name="val">The number whose base 10 logarithm is to be calculated.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Log10(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Log10, val);
        }

        /// <summary>
        /// Returns a specified number raised to the specified power.
        /// </summary>
        /// <param name="val1">A floating-point number to be raised to a power.</param>
        /// <param name="val2">A floating-point number that specifies a power.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Pow(ScalarNode val1, ScalarNode val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::Pow, val1, val2);
        }

        /// <summary>
        /// Rounds a floating point value to the nearest integral value.
        /// </summary>
        /// <param name="val">The floating point number to round.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Round(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Round, val);
        }

        /// <summary>
        /// Returns the sine of the specified angle (in radians).
        /// </summary>
        /// <param name="val">An angle, measured in radians.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Sin(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Sin, val);
        }

        /// <summary>
        /// Returns the specified number multiplied by itself.
        /// </summary>
        /// <param name="val">The floating point number to square.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Square(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Square, val);
        }

        /// <summary>
        /// Returns the square root of a specified number.
        /// </summary>
        /// <param name="val">The number whose square root is to be returned.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Sqrt(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Sqrt, val);
        }

        /// <summary>
        /// Returns the tangent of the specified angle (in radians).
        /// </summary>
        /// <param name="val">An angle, measured in radians.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Tan(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Tan, val);
        }

        /// <summary>
        /// Converts an angle in radians to degrees as: val*180/PI.
        /// </summary>
        /// <param name="val">A floating point value that represents an angle in radians.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::ToDegrees(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::ToDegrees, val);
        }

        /// <summary>
        /// Converts an angle in degrees to radians as: val*PI/180.
        /// </summary>
        /// <param name="val">A floating point value that represents an angle in degrees.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::ToRadians(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::ToRadians, val);
        }

        // System.Numerics functions

        /// <summary>
        /// Returns the absolute value of the specified input. For vectors, the absolute value of each subchannel is returned.
        /// </summary>
        /// <param name="val">The input value.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Abs(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Absolute, val);
        }

        /// <summary>
        /// Returns the absolute value of the specified input. For vectors, the absolute value of each subchannel is returned.
        /// </summary>
        /// <param name="val">The input value.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Abs(Vector2Node val)
        {
            return Function<Vector2Node>(ExpressionNodeType::Absolute, val);
        }

        /// <summary>
        /// Returns the absolute value of the specified input. For vectors, the absolute value of each subchannel is returned.
        /// </summary>
        /// <param name="val">The input value.</param>
        /// <returns>Vector3Node.</returns>
        Vector3Node ExpressionFunctions::Abs(Vector3Node val)
        {
            return Function<Vector3Node>(ExpressionNodeType::Absolute, val);
        }

        /// <summary>
        /// Returns the absolute value of the specified input. For vectors, the absolute value of each subchannel is returned.
        /// .
        /// </summary>
        /// <param name="val">The input value.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Abs(Vector4Node val)
        {
            return Function<Vector4Node>(ExpressionNodeType::Absolute, val);
        }

        /// <summary>
        /// Restricts a value to be within a specified range. For vectors, each subchannel is clamped.
        /// </summary>
        /// <param name="val">The value to clamp.</param>
        /// <param name="min">The specified minimum range.</param>
        /// <param name="max">The specified maximum range.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Clamp(ScalarNode val, ScalarNode min, ScalarNode max)
        {
            return Function<ScalarNode>(ExpressionNodeType::Clamp, val, min, max);
        }

        /// <summary>
        /// Restricts a value to be within a specified range. For vectors, each subchannel is clamped.
        /// </summary>
        /// <param name="val">The value to clamp.</param>
        /// <param name="min">The specified minimum range.</param>
        /// <param name="max">The specified maximum range.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Clamp(Vector2Node val, Vector2Node min, Vector2Node max)
        {
            return Function<Vector2Node>(ExpressionNodeType::Clamp, val, min, max);
        }

        /// <summary>
        /// Restricts a value to be within a specified range. For vectors, each subchannel is clamped.
        /// </summary>
        /// <param name="val">The value to clamp.</param>
        /// <param name="min">The specified minimum range.</param>
        /// <param name="max">The specified maximum range.</param>
        /// <returns>Vector3Node.</returns>
        Vector3Node ExpressionFunctions::Clamp(Vector3Node val, Vector3Node min, Vector3Node max)
        {
            return Function<Vector3Node>(ExpressionNodeType::Clamp, val, min, max);
        }

        /// <summary>
        /// Restricts a value to be within a specified range. For vectors, each subchannel is clamped.
        /// </summary>
        /// <param name="val">The value to clamp.</param>
        /// <param name="min">The specified minimum range.</param>
        /// <param name="max">The specified maximum range.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Clamp(Vector4Node val, Vector4Node min, Vector4Node max)
        {
            return Function<Vector4Node>(ExpressionNodeType::Clamp, val, min, max);
        }

        /// <summary>
        /// Linearly interpolates between two colors in the default color space.
        /// </summary>
        /// <param name="val1">Color source value 1.</param>
        /// <param name="val2">Color source value 2.</param>
        /// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
        /// <returns>ColorNode.</returns>
        ColorNode ExpressionFunctions::ColorLerp(ColorNode val1, ColorNode val2, ScalarNode progress)
        {
            return Function<ColorNode>(ExpressionNodeType::ColorLerp, val1, val2, progress);
        }

        /// <summary>
        /// Linearly interpolates between two colors in the HSL color space.
        /// </summary>
        /// <param name="val1">Color source value 1.</param>
        /// <param name="val2">Color source value 2.</param>
        /// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
        /// <returns>ColorNode.</returns>
        ColorNode ExpressionFunctions::ColorLerpHsl(ColorNode val1, ColorNode val2, ScalarNode progress)
        {
            return Function<ColorNode>(ExpressionNodeType::ColorLerpHsl, val1, val2, progress);
        }

        /// <summary>
        /// Linearly interpolates between two colors in the RBG color space.
        /// </summary>
        /// <param name="val1">Color source value 1.</param>
        /// <param name="val2">Color source value 2.</param>
        /// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
        /// <returns>ColorNode.</returns>
        ColorNode ExpressionFunctions::ColorLerpRgb(ColorNode val1, ColorNode val2, ScalarNode progress)
        {
            return Function<ColorNode>(ExpressionNodeType::ColorLerpRgb, val1, val2, progress);
        }

        /// <summary>
        /// Concatenates two Quaternions; the result represents the first rotation followed by the second rotation.
        /// </summary>
        /// <param name="val1">The first quaternion rotation in the series.</param>
        /// <param name="val2">The second quaternion rotation in the series.</param>
        /// <returns>QuaternionNode.</returns>
        QuaternionNode ExpressionFunctions::Concatenate(QuaternionNode val1, QuaternionNode val2)
        {
            return Function<QuaternionNode>(ExpressionNodeType::Concatenate, val1, val2);
        }

        /// <summary>
        /// Returns the distance between two vectors as: sqrt((x1-x2)^2 + (y1-y2)^2 + ...).
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Distance(ScalarNode val1, ScalarNode val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::Distance, val1, val2);
        }

        /// <summary>
        /// Returns the distance between two vectors as: sqrt((x1-x2)^2 + (y1-y2)^2 + ...).
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Distance(Vector2Node val1, Vector2Node val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::Distance, val1, val2);
        }

        /// <summary>
        /// Returns the distance between two vectors as: sqrt((x1-x2)^2 + (y1-y2)^2 + ...).
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Distance(Vector3Node val1, Vector3Node val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::Distance, val1, val2);
        }

        /// <summary>
        /// Returns the distance between two vectors as: sqrt((x1-x2)^2 + (y1-y2)^2 + ...).
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Distance(Vector4Node val1, Vector4Node val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::Distance, val1, val2);
        }

        /// <summary>
        /// Returns the squared distance between two vectors as: ((x1-x2)^2 + (y1-y2)^2 + ...).
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::DistanceSquared(ScalarNode val1, ScalarNode val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::DistanceSquared, val1, val2);
        }

        /// <summary>
        /// Returns the squared distance between two vectors as: ((x1-x2)^2 + (y1-y2)^2 + ...).
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::DistanceSquared(Vector2Node val1, Vector2Node val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::DistanceSquared, val1, val2);
        }

        /// <summary>
        /// Returns the squared distance between two vectors as: ((x1-x2)^2 + (y1-y2)^2 + ...).
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::DistanceSquared(Vector3Node val1, Vector3Node val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::DistanceSquared, val1, val2);
        }

        /// <summary>
        /// Returns the squared distance between two vectors as: ((x1-x2)^2 + (y1-y2)^2 + ...).
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::DistanceSquared(Vector4Node val1, Vector4Node val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::DistanceSquared, val1, val2);
        }

        /// <summary>
        /// Returns the inverse of the specified matrix.
        /// </summary>
        /// <param name="val">The matrix to invert.</param>
        /// <returns>Matrix3x2Node.</returns>
        Matrix3x2Node ExpressionFunctions::Inverse(Matrix3x2Node val)
        {
            return Function<Matrix3x2Node>(ExpressionNodeType::Inverse, val);
        }

        /// <summary>
        /// Returns the inverse of the specified matrix.
        /// </summary>
        /// <param name="val">The matrix to invert.</param>
        /// <returns>Matrix4x4Node.</returns>
        Matrix4x4Node ExpressionFunctions::Inverse(Matrix4x4Node val)
        {
            return Function<Matrix4x4Node>(ExpressionNodeType::Inverse, val);
        }

        /// <summary>
        /// Returns the length of the vector as: sqrt(x^2 + y^2 + ...).
        /// </summary>
        /// <param name="val">Vector value to return the length of.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Length(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Length, val);
        }

        /// <summary>
        /// Returns the length of the vector as: sqrt(x^2 + y^2 + ...).
        /// </summary>
        /// <param name="val">Vector value to return the length of.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Length(Vector2Node val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Length, val);
        }

        /// <summary>
        /// Returns the length of the vector as: sqrt(x^2 + y^2 + ...).
        /// </summary>
        /// <param name="val">Vector value to return the length of.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Length(Vector3Node val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Length, val);
        }

        /// <summary>
        /// Returns the length of the vector as: sqrt(x^2 + y^2 + ...).
        /// </summary>
        /// <param name="val">Vector value to return the length of.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Length(Vector4Node val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Length, val);
        }

        /// <summary>
        /// Returns the length of the vector as: sqrt(x^2 + y^2 + ...).
        /// </summary>
        /// <param name="val">Vector value to return the length of.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Length(QuaternionNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::Length, val);
        }

        /// <summary>
        /// Returns the squared length of the vector as: (x^2 + y^2 + ...).
        /// </summary>
        /// <param name="val">Vector value to return the length squared of.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::LengthSquared(ScalarNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::LengthSquared, val);
        }

        /// <summary>
        /// Returns the squared length of the vector as: (x^2 + y^2 + ...).
        /// </summary>
        /// <param name="val">Vector value to return the length squared of.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::LengthSquared(Vector2Node val)
        {
            return Function<ScalarNode>(ExpressionNodeType::LengthSquared, val);
        }

        /// <summary>
        /// Returns the squared length of the vector as: (x^2 + y^2 + ...).
        /// </summary>
        /// <param name="val">Vector value to return the length squared of.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::LengthSquared(Vector3Node val)
        {
            return Function<ScalarNode>(ExpressionNodeType::LengthSquared, val);
        }

        /// <summary>
        /// Returns the squared length of the vector as: (x^2 + y^2 + ...).
        /// </summary>
        /// <param name="val">Vector value to return the length squared of.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::LengthSquared(Vector4Node val)
        {
            return Function<ScalarNode>(ExpressionNodeType::LengthSquared, val);
        }

        /// <summary>
        /// Returns the squared length of the vector as: (x^2 + y^2 + ...).
        /// </summary>
        /// <param name="val">Vector value to return the length squared of.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::LengthSquared(QuaternionNode val)
        {
            return Function<ScalarNode>(ExpressionNodeType::LengthSquared, val);
        }

        /// <summary>
        /// Linearly interpolates between two vectors as: Output.x = x1 + (x2-x1)*progress.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Lerp(ScalarNode val1, ScalarNode val2, ScalarNode progress)
        {
            return Function<ScalarNode>(ExpressionNodeType::Lerp, val1, val2, progress);
        }

        /// <summary>
        /// Linearly interpolates between two vectors as: Output.x = x1 + (x2-x1)*progress.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Lerp(Vector2Node val1, Vector2Node val2, ScalarNode progress)
        {
            return Function<Vector2Node>(ExpressionNodeType::Lerp, val1, val2, progress);
        }

        /// <summary>
        /// Linearly interpolates between two vectors as: Output.x = x1 + (x2-x1)*progress.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
        /// <returns>Vector3Node.</returns>
        Vector3Node ExpressionFunctions::Lerp(Vector3Node val1, Vector3Node val2, ScalarNode progress)
        {
            return Function<Vector3Node>(ExpressionNodeType::Lerp, val1, val2, progress);
        }

        /// <summary>
        /// Linearly interpolates between two vectors as: Output.x = x1 + (x2-x1)*progress.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Lerp(Vector4Node val1, Vector4Node val2, ScalarNode progress)
        {
            return Function<Vector4Node>(ExpressionNodeType::Lerp, val1, val2, progress);
        }

        /// <summary>
        /// Returns the maximum of two values. For vectors, the max of each subchannel is returned.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Max(ScalarNode val1, ScalarNode val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::Max, val1, val2);
        }

        /// <summary>
        /// Returns the maximum of two values. For vectors, the max of each subchannel is returned.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Max(Vector2Node val1, Vector2Node val2)
        {
            return Function<Vector2Node>(ExpressionNodeType::Max, val1, val2);
        }

        /// <summary>
        /// Returns the maximum of two values. For vectors, the max of each subchannel is returned.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>Vector3Node.</returns>
        Vector3Node ExpressionFunctions::Max(Vector3Node val1, Vector3Node val2)
        {
            return Function<Vector3Node>(ExpressionNodeType::Max, val1, val2);
        }

        /// <summary>
        /// Returns the maximum of two values. For vectors, the max of each subchannel is returned.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Max(Vector4Node val1, Vector4Node val2)
        {
            return Function<Vector4Node>(ExpressionNodeType::Max, val1, val2);
        }

        /// <summary>
        /// Returns the minimum of two values. For vectors, the min of each subchannel is returned.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Min(ScalarNode val1, ScalarNode val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::Min, val1, val2);
        }

        /// <summary>
        /// Returns the minimum of two values. For vectors, the min of each subchannel is returned.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Min(Vector2Node val1, Vector2Node val2)
        {
            return Function<Vector2Node>(ExpressionNodeType::Min, val1, val2);
        }

        /// <summary>
        /// Returns the minimum of two values. For vectors, the min of each subchannel is returned.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>Vector3Node.</returns>
        Vector3Node ExpressionFunctions::Min(Vector3Node val1, Vector3Node val2)
        {
            return Function<Vector3Node>(ExpressionNodeType::Min, val1, val2);
        }

        /// <summary>
        /// Returns the minimum of two values. For vectors, the min of each subchannel is returned.
        /// </summary>
        /// <param name="val1">Source value 1.</param>
        /// <param name="val2">Source value 2.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Min(Vector4Node val1, Vector4Node val2)
        {
            return Function<Vector4Node>(ExpressionNodeType::Min, val1, val2);
        }

        /// <summary>
        /// Returns the remainder resulting from dividing val1/val2. For vectors, the remainder for each subchannel is returned.
        /// </summary>
        /// <param name="val1">The numerator value.</param>
        /// <param name="val2">The denominator value.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Mod(ScalarNode val1, ScalarNode val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::Modulus, val1, val2);
        }

        /// <summary>
        /// Returns the remainder resulting from dividing val1/val2. For vectors, the remainder for each subchannel is returned.
        /// </summary>
        /// <param name="val1">The numerator value.</param>
        /// <param name="val2">The denominator value.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Mod(Vector2Node val1, Vector2Node val2)
        {
            return Function<Vector2Node>(ExpressionNodeType::Modulus, val1, val2);
        }

        /// <summary>
        /// Returns the remainder resulting from dividing val1/val2. For vectors, the remainder for each subchannel is returned.
        /// </summary>
        /// <param name="val1">The numerator value.</param>
        /// <param name="val2">The denominator value.</param>
        /// <returns>Vector3Node.</returns>
        Vector3Node ExpressionFunctions::Mod(Vector3Node val1, Vector3Node val2)
        {
            return Function<Vector3Node>(ExpressionNodeType::Modulus, val1, val2);
        }

        /// <summary>
        /// Returns the remainder resulting from dividing val1/val2. For vectors, the remainder for each subchannel is returned.
        /// </summary>
        /// <param name="val1">The numerator value.</param>
        /// <param name="val2">The denominator value.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Mod(Vector4Node val1, Vector4Node val2)
        {
            return Function<Vector4Node>(ExpressionNodeType::Modulus, val1, val2);
        }

        /// <summary>
        /// Returns the normalized version of a vector.
        /// </summary>
        /// <param name="val">Vector value to normalize.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Normalize(Vector2Node val)
        {
            return Function<Vector2Node>(ExpressionNodeType::Normalize, val);
        }

        /// <summary>
        /// Returns the normalized version of a vector.
        /// </summary>
        /// <param name="val">Vector value to normalize.</param>
        /// <returns>Vector3Node.</returns>
        Vector3Node ExpressionFunctions::Normalize(Vector3Node val)
        {
            return Function<Vector3Node>(ExpressionNodeType::Normalize, val);
        }

        /// <summary>
        /// Returns the normalized version of a vector.
        /// </summary>
        /// <param name="val">Vector value to normalize.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Normalize(Vector4Node val)
        {
            return Function<Vector4Node>(ExpressionNodeType::Normalize, val);
        }

        /// <summary>
        /// Returns the normalized version of a vector.
        /// </summary>
        /// <param name="val">Vector value to normalize.</param>
        /// <returns>QuaternionNode.</returns>
        QuaternionNode ExpressionFunctions::Normalize(QuaternionNode val)
        {
            return Function<QuaternionNode>(ExpressionNodeType::Normalize, val);
        }

        /// <summary>
        /// Multiply each subchannel of the specified vector/matrix by a float value.
        /// </summary>
        /// <param name="val1">Source value to scale.</param>
        /// <param name="val2">Scaling value.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Scale(ScalarNode val1, ScalarNode val2)
        {
            return Function<ScalarNode>(ExpressionNodeType::Scale, val1, val2);
        }

        /// <summary>
        /// Multiply each subchannel of the specified vector/matrix by a float value.
        /// </summary>
        /// <param name="val1">Source value to scale.</param>
        /// <param name="val2">Scaling value.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Scale(Vector2Node val1, ScalarNode val2)
        {
            return Function<Vector2Node>(ExpressionNodeType::Scale, val1, val2);
        }

        /// <summary>
        /// Multiply each subchannel of the specified vector/matrix by a float value.
        /// </summary>
        /// <param name="val1">Source value to scale.</param>
        /// <param name="val2">Scaling value.</param>
        /// <returns>Vector3Node.</returns>
        Vector3Node ExpressionFunctions::Scale(Vector3Node val1, ScalarNode val2)
        {
            return Function<Vector3Node>(ExpressionNodeType::Scale, val1, val2);
        }

        /// <summary>
        /// Multiply each subchannel of the specified vector/matrix by a float value.
        /// </summary>
        /// <param name="val1">Source value to scale.</param>
        /// <param name="val2">Scaling value.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Scale(Vector4Node val1, ScalarNode val2)
        {
            return Function<Vector4Node>(ExpressionNodeType::Scale, val1, val2);
        }

        /// <summary>
        /// Multiply each subchannel of the specified vector/matrix by a float value.
        /// </summary>
        /// <param name="val1">Source value to scale.</param>
        /// <param name="val2">Scaling value.</param>
        /// <returns>Matrix3x2Node.</returns>
        Matrix3x2Node ExpressionFunctions::Scale(Matrix3x2Node val1, ScalarNode val2)
        {
            return Function<Matrix3x2Node>(ExpressionNodeType::Scale, val1, val2);
        }

        /// <summary>
        /// Multiply each subchannel of the specified vector/matrix by a float value.
        /// </summary>
        /// <param name="val1">Source value to scale.</param>
        /// <param name="val2">Scaling value.</param>
        /// <returns>Matrix4x4Node.</returns>
        Matrix4x4Node ExpressionFunctions::Scale(Matrix4x4Node val1, ScalarNode val2)
        {
            return Function<Matrix4x4Node>(ExpressionNodeType::Scale, val1, val2);
        }

        /// <summary>
        /// Spherically interpolates between two quaternions.
        /// </summary>
        /// <param name="val1">Quaternion source value 1.</param>
        /// <param name="val2">Quaternion source value 2.</param>
        /// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
        /// <returns>QuaternionNode.</returns>
        QuaternionNode ExpressionFunctions::Slerp(QuaternionNode val1, QuaternionNode val2, ScalarNode progress)
        {
            return Function<QuaternionNode>(ExpressionNodeType::Slerp, val1, val2, progress);
        }

        /// <summary>
        /// Transforms a vector by the specified matrix.
        /// </summary>
        /// <param name="val1">Vector to be transformed.</param>
        /// <param name="val2">The transformation matrix.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Transform(Vector2Node val1, Matrix3x2Node val2)
        {
            return Function<Vector2Node>(ExpressionNodeType::Transform, val1, val2);
        }

        /// <summary>
        /// Transforms a vector by the specified matrix.
        /// </summary>
        /// <param name="val1">Vector to be transformed.</param>
        /// <param name="val2">The transformation matrix.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Transform(Vector4Node val1, Matrix4x4Node val2)
        {
            return Function<Vector4Node>(ExpressionNodeType::Transform, val1, val2);
        }

        // System.Numerics Type Constructors

        /// <summary>
        /// Creates a vector whose subchannels have the specified values.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Vector2(ScalarNode x, ScalarNode y)
        {
            return Function<Vector2Node>(ExpressionNodeType::Vector2, x, y);
        }

        /// <summary>
        /// Creates a vector whose subchannels have the specified values.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <param name="z">The z.</param>
        /// <returns>Vector3Node.</returns>
        Vector3Node ExpressionFunctions::Vector3(ScalarNode x, ScalarNode y, ScalarNode z)
        {
            return Function<Vector3Node>(ExpressionNodeType::Vector3, x, y, z);
        }

        /// <summary>
        /// Creates a vector whose subchannels have the specified values.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <param name="z">The z.</param>
        /// <param name="w">The w.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Vector4(ScalarNode x, ScalarNode y, ScalarNode z, ScalarNode w)
        {
            return Function<Vector4Node>(ExpressionNodeType::Vector4, x, y, z, w);
        }

        /// <summary>
        /// Creates a color in the HSL format.
        /// </summary>
        /// <param name="h">Hue</param>
        /// <param name="s">Saturation</param>
        /// <param name="l">Luminosity</param>
        /// <returns>ColorNode.</returns>
        ColorNode ExpressionFunctions::ColorHsl(ScalarNode h, ScalarNode s, ScalarNode l)
        {
            return Function<ColorNode>(ExpressionNodeType::ColorHsl, h, s, l);
        }

        /// <summary>
        /// Creates a Color in the ARGB format.
        /// </summary>
        /// <param name="alpha">The alpha.</param>
        /// <param name="red">The red.</param>
        /// <param name="green">The green.</param>
        /// <param name="blue">The blue.</param>
        /// <returns>ColorNode.</returns>
        ColorNode ExpressionFunctions::ColorRgb(ScalarNode alpha, ScalarNode red, ScalarNode green, ScalarNode blue)
        {
            return Function<ColorNode>(ExpressionNodeType::ColorRgb, alpha, red, green, blue);
        }

        /// <summary>
        /// Creates a quaternion whose subchannels have the specified values.
        /// </summary>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <param name="z">The z.</param>
        /// <param name="w">The w.</param>
        /// <returns>QuaternionNode.</returns>
        QuaternionNode ExpressionFunctions::Quaternion(ScalarNode x, ScalarNode y, ScalarNode z, ScalarNode w)
        {
            return Function<QuaternionNode>(ExpressionNodeType::Quaternion, x, y, z, w);
        }

        /// <summary>
        /// Creates a matrix whose subchannels have the specified values.
        /// </summary>
        /// <param name="channel11">The channel11.</param>
        /// <param name="channel12">The channel12.</param>
        /// <param name="channel21">The channel21.</param>
        /// <param name="channel22">The channel22.</param>
        /// <param name="channel31">The channel31.</param>
        /// <param name="channel32">The channel32.</param>
        /// <returns>Matrix3x2Node.</returns>
        Matrix3x2Node ExpressionFunctions::Matrix3x2(ScalarNode channel11, ScalarNode channel12, ScalarNode channel21, ScalarNode channel22, ScalarNode channel31, ScalarNode channel32)
        {
            return Function<Matrix3x2Node>(ExpressionNodeType::Matrix3x2, channel11, channel12, channel21, channel22, channel31, channel32);
        }

        /// <summary>
        /// Creates a matrix whose subchannels have the specified values.
        /// </summary>
        /// <param name="channel11">The channel11.</param>
        /// <param name="channel12">The channel12.</param>
        /// <param name="channel13">The channel13.</param>
        /// <param name="channel14">The channel14.</param>
        /// <param name="channel21">The channel21.</param>
        /// <param name="channel22">The channel22.</param>
        /// <param name="channel23">The channel23.</param>
        /// <param name="channel24">The channel24.</param>
        /// <param name="channel31">The channel31.</param>
        /// <param name="channel32">The channel32.</param>
        /// <param name="channel33">The channel33.</param>
        /// <param name="channel34">The channel34.</param>
        /// <param name="channel41">The channel41.</param>
        /// <param name="channel42">The channel42.</param>
        /// <param name="channel43">The channel43.</param>
        /// <param name="channel44">The channel44.</param>
        /// <returns>Matrix4x4Node.</returns>
        Matrix4x4Node ExpressionFunctions::Matrix4x4(ScalarNode channel11, ScalarNode channel12, ScalarNode channel13, ScalarNode channel14,
            ScalarNode channel21, ScalarNode channel22, ScalarNode channel23, ScalarNode channel24,
            ScalarNode channel31, ScalarNode channel32, ScalarNode channel33, ScalarNode channel34,
            ScalarNode channel41, ScalarNode channel42, ScalarNode channel43, ScalarNode channel44)
        {
            return Function<Matrix4x4Node>(ExpressionNodeType::Matrix4x4, channel11, channel12, channel13, channel14, channel21, channel22, channel23, channel24, channel31, channel32, channel33, channel34, channel41, channel42, channel43, channel44);
        }

        /// <summary>
        /// Creates a 4x4 matrix from a 3x2 matrix.
        /// </summary>
        /// <param name="val">The value.</param>
        /// <returns>Matrix4x4Node.</returns>
        Matrix4x4Node ExpressionFunctions::Matrix4x4(Matrix3x2Node val)
        {
            return Function<Matrix4x4Node>(
                ExpressionNodeType::Matrix4x4,
                val.Channel11(), val.Channel12(), (ScalarNode)0, (ScalarNode)0,
                val.Channel21(), val.Channel22(), (ScalarNode)0, (ScalarNode)0,
                (ScalarNode)0, (ScalarNode)0, (ScalarNode)1, (ScalarNode)0,
                val.Channel31(), val.Channel32(), (ScalarNode)0, (ScalarNode)1);
        }

        /// <summary>
        /// Creates a translation matrix from the specified vector.
        /// </summary>
        /// <param name="val">Source translation vector.</param>
        /// <returns>Matrix3x2Node.</returns>
        Matrix3x2Node ExpressionFunctions::CreateTranslation(Vector2Node val)
        {
            return Function<Matrix3x2Node>(ExpressionNodeType::Matrix3x2FromTranslation, val);
        }

        /// <summary>
        /// Creates a translation matrix from the specified vector.
        /// </summary>
        /// <param name="val">Source translation vector.</param>
        /// <returns>Matrix4x4Node.</returns>
        Matrix4x4Node ExpressionFunctions::CreateTranslation(Vector3Node val)
        {
            return Function<Matrix4x4Node>(ExpressionNodeType::Matrix4x4FromTranslation, val);
        }

        /// <summary>
        /// Creates a scale matrix from the specified vector scale.
        /// </summary>
        /// <param name="val">Source scaling vector.</param>
        /// <returns>Matrix3x2Node.</returns>
        Matrix3x2Node ExpressionFunctions::CreateScale(Vector2Node val)
        {
            return Function<Matrix3x2Node>(ExpressionNodeType::Matrix3x2FromScale, val);
        }

        /// <summary>
        /// Creates a scale matrix from the specified vector scale.
        /// </summary>
        /// <param name="val">Source scaling vector.</param>
        /// <returns>Matrix4x4Node.</returns>
        Matrix4x4Node ExpressionFunctions::CreateScale(Vector3Node val)
        {
            return Function<Matrix4x4Node>(ExpressionNodeType::Matrix4x4FromScale, val);
        }

        /// <summary>
        /// Creates a skew matrix from the specified angles in radians.
        /// </summary>
        /// <param name="xAngle">X angle, in radians.</param>
        /// <param name="yAngle">Y angle, in radians.</param>
        /// <param name="centerPoint">The centerpoint for the operation.</param>
        /// <returns>Matrix3x2Node.</returns>
        Matrix3x2Node ExpressionFunctions::CreateSkew(ScalarNode xAngle, ScalarNode yAngle, Vector2Node centerPoint)
        {
            return Function<Matrix3x2Node>(ExpressionNodeType::Matrix3x2FromSkew, xAngle, yAngle, centerPoint);
        }

        /// <summary>
        /// Creates a rotation matrix using the given rotation in radians.
        /// </summary>
        /// <param name="angle">Angle, in radians.</param>
        /// <returns>Matrix3x2Node.</returns>
        Matrix3x2Node ExpressionFunctions::CreateRotation(ScalarNode angle)
        {
            return Function<Matrix3x2Node>(ExpressionNodeType::Matrix3x2FromRotation, angle);
        }

        /// <summary>
        /// Creates a matrix that rotates around an arbitrary vector.
        /// </summary>
        /// <param name="axis">Rotation axis</param>
        /// <param name="angle">Angle, in radians.</param>
        /// <returns>Matrix4x4Node.</returns>
        Matrix4x4Node ExpressionFunctions::CreateMatrix4x4FromAxisAngle(Vector3Node axis, ScalarNode angle)
        {
            return Function<Matrix4x4Node>(ExpressionNodeType::Matrix4x4FromAxisAngle, axis, angle);
        }

        /// <summary>
        /// Creates a quaternion that rotates around an arbitrary vector.
        /// </summary>
        /// <param name="axis">Rotation axis</param>
        /// <param name="angle">Angle, in radians.</param>
        /// <returns>QuaternionNode.</returns>
        QuaternionNode ExpressionFunctions::CreateQuaternionFromAxisAngle(Vector3Node axis, ScalarNode angle)
        {
            return Function<QuaternionNode>(ExpressionNodeType::QuaternionFromAxisAngle, axis, angle);
        }

        /// <summary>
        /// Performs a logical AND operation on two boolean values as: val1 &amp;&amp; val2.
        /// </summary>
        /// <param name="val1">The val1.</param>
        /// <param name="val2">The val2.</param>
        /// <returns>BooleanNode.</returns>
        BooleanNode ExpressionFunctions::And(BooleanNode val1, BooleanNode val2)
        {
            return Function<BooleanNode>(ExpressionNodeType::And, val1, val2);
        }

        /// <summary>
        /// Performs a logical OR operation on two boolean values as: val1 || val2.
        /// </summary>
        /// <param name="val1">The val1.</param>
        /// <param name="val2">The val2.</param>
        /// <returns>BooleanNode.</returns>
        BooleanNode ExpressionFunctions::Or(BooleanNode val1, BooleanNode val2)
        {
            return Function<BooleanNode>(ExpressionNodeType::Or, val1, val2);
        }

        /// <summary>
        /// Performs a logical NOT operation on a specified boolean value as: !val.
        /// </summary>
        /// <param name="val">The value.</param>
        /// <returns>BooleanNode.</returns>
        BooleanNode ExpressionFunctions::Not(BooleanNode val)
        {
            return Function<BooleanNode>(ExpressionNodeType::Not, val);
        }

        /// <summary>
        /// Returns one of two values, depending on the value of the boolean condition.
        /// </summary>
        /// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
        /// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
        /// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
        /// <returns>ScalarNode.</returns>
        ScalarNode ExpressionFunctions::Conditional(BooleanNode condition, ScalarNode trueCase, ScalarNode falseCase)
        {
            return Function<ScalarNode>(ExpressionNodeType::Conditional, condition, trueCase, falseCase);
        }

        /// <summary>
        /// Returns one of two values, depending on the value of the boolean condition.
        /// </summary>
        /// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
        /// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
        /// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
        /// <returns>Vector2Node.</returns>
        Vector2Node ExpressionFunctions::Conditional(BooleanNode condition, Vector2Node trueCase, Vector2Node falseCase)
        {
            return Function<Vector2Node>(ExpressionNodeType::Conditional, condition, trueCase, falseCase);
        }

        /// <summary>
        /// Returns one of two values, depending on the value of the boolean condition.
        /// </summary>
        /// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
        /// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
        /// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
        /// <returns>Vector3Node.</returns>
        Vector3Node ExpressionFunctions::Conditional(BooleanNode condition, Vector3Node trueCase, Vector3Node falseCase)
        {
            return Function<Vector3Node>(ExpressionNodeType::Conditional, condition, trueCase, falseCase);
        }

        /// <summary>
        /// Returns one of two values, depending on the value of the boolean condition.
        /// </summary>
        /// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
        /// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
        /// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
        /// <returns>Vector4Node.</returns>
        Vector4Node ExpressionFunctions::Conditional(BooleanNode condition, Vector4Node trueCase, Vector4Node falseCase)
        {
            return Function<Vector4Node>(ExpressionNodeType::Conditional, condition, trueCase, falseCase);
        }

        /// <summary>
        /// Returns one of two values, depending on the value of the boolean condition.
        /// </summary>
        /// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
        /// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
        /// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
        /// <returns>ColorNode.</returns>
        ColorNode ExpressionFunctions::Conditional(BooleanNode condition, ColorNode trueCase, ColorNode falseCase)
        {
            return Function<ColorNode>(ExpressionNodeType::Conditional, condition, trueCase, falseCase);
        }

        /// <summary>
        /// Returns one of two values, depending on the value of the boolean condition.
        /// </summary>
        /// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
        /// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
        /// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
        /// <returns>QuaternionNode.</returns>
        QuaternionNode ExpressionFunctions::Conditional(BooleanNode condition, QuaternionNode trueCase, QuaternionNode falseCase)
        {
            return Function<QuaternionNode>(ExpressionNodeType::Conditional, condition, trueCase, falseCase);
        }

        /// <summary>
        /// Returns one of two values, depending on the value of the boolean condition.
        /// </summary>
        /// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
        /// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
        /// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
        /// <returns>Matrix3x2Node.</returns>
        Matrix3x2Node ExpressionFunctions::Conditional(BooleanNode condition, Matrix3x2Node trueCase, Matrix3x2Node falseCase)
        {
            return Function<Matrix3x2Node>(ExpressionNodeType::Conditional, condition, trueCase, falseCase);
        }

        /// <summary>
        /// Returns one of two values, depending on the value of the boolean condition.
        /// </summary>
        /// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
        /// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
        /// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
        /// <returns>Matrix4x4Node.</returns>
        Matrix4x4Node ExpressionFunctions::Conditional(BooleanNode condition, Matrix4x4Node trueCase, Matrix4x4Node falseCase)
        {
            return Function<Matrix4x4Node>(ExpressionNodeType::Conditional, condition, trueCase, falseCase);
        }

        /// <summary>
        /// Gets the type of the node information from.
        /// </summary>
        /// <param name="type">The type.</param>
        /// <returns>ExpressionNodeInfo.</returns>
        ExpressionFunctions::ExpressionNodeInfo ExpressionFunctions::GetNodeInfoFromType(ExpressionNodeType type)
        {
            return _expressionNodeInfo.at(type);
        }
}
