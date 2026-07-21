#pragma once

#include "ExpressionNodes/ExpressionNode.h"
#include "OperationType.h"
#include "ExpressionNodes/BooleanNode.h"
#include "ExpressionNodes/QuaternionNode.h"
#include "ExpressionNodes/ColorNode.h"
#include "ExpressionNodes/ScalarNode.h"
#include "ExpressionNodes/Vector2Node.h"
#include "ExpressionNodes/Vector3Node.h"
#include "ExpressionNodes/Vector4Node.h"
#include "ExpressionNodes/Matrix3x2Node.h"
#include "ExpressionNodes/Matrix4x4Node.h"
#include "ExpressionNodes/ExpressionNodeType.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <functional>
#include <map>
#include <optional>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::Expressions
{
	struct ExpressionNodeInfo;

	/// <summary>
	/// Class ExpressionFunctions.
	/// </summary>
	class ExpressionFunctions
	{
		/// <summary>
		/// Returns the angle (in radians) whose cosine is the specified number.
		/// </summary>
		/// <param name="val">Value between -1 and 1, for which to calculate the arccosine (the inverse cosine).</param>
		/// <returns>ScalarNode.</returns>
	public:
		static ScalarNode ACos(ScalarNode val);

		/// <summary>
		/// Returns the angle (in radians) whose sine is the specified number.
		/// </summary>
		/// <param name="val">Value between -1 and 1, for which to calculate the arcsine (the inverse sine).</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode ASin(ScalarNode val);

		/// <summary>
		/// Returns the angle (in radians) whose tangent is the specified number.
		/// </summary>
		/// <param name="val">Value for which to calculate the arctan (the inverse tan).</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode ATan(ScalarNode val);

		/// <summary>
		/// Returns the smallest integral value that is greater than or equal to the specified value.
		/// </summary>
		/// <param name="val">The floating point number to round.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Ceil(ScalarNode val);

		/// <summary>
		/// Returns the cosine of the specified angle (in radians).
		/// </summary>
		/// <param name="val">An angle, measured in radians.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Cos(ScalarNode val);

		/// <summary>
		/// Returns the largest integer less than or equal to the specified value.
		/// </summary>
		/// <param name="val">The floating point number to round.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Floor(ScalarNode val);

		/// <summary>
		/// Returns the natural (base e) logarithm of a specified number.
		/// </summary>
		/// <param name="val">The number whose natural logarithm is to be returned.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Ln(ScalarNode val);

		/// <summary>
		/// Returns the base 10 logarithm of a specified number.
		/// </summary>
		/// <param name="val">The number whose base 10 logarithm is to be calculated.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Log10(ScalarNode val);

		/// <summary>
		/// Returns a specified number raised to the specified power.
		/// </summary>
		/// <param name="val1">A floating-point number to be raised to a power.</param>
		/// <param name="val2">A floating-point number that specifies a power.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Pow(ScalarNode val1, ScalarNode val2);

		/// <summary>
		/// Rounds a floating point value to the nearest integral value.
		/// </summary>
		/// <param name="val">The floating point number to round.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Round(ScalarNode val);

		/// <summary>
		/// Returns the sine of the specified angle (in radians).
		/// </summary>
		/// <param name="val">An angle, measured in radians.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Sin(ScalarNode val);

		/// <summary>
		/// Returns the specified number multiplied by itself.
		/// </summary>
		/// <param name="val">The floating point number to square.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Square(ScalarNode val);

		/// <summary>
		/// Returns the square root of a specified number.
		/// </summary>
		/// <param name="val">The number whose square root is to be returned.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Sqrt(ScalarNode val);

		/// <summary>
		/// Returns the tangent of the specified angle (in radians).
		/// </summary>
		/// <param name="val">An angle, measured in radians.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Tan(ScalarNode val);

		/// <summary>
		/// Converts an angle in radians to degrees as: val*180/PI.
		/// </summary>
		/// <param name="val">A floating point value that represents an angle in radians.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode ToDegrees(ScalarNode val);

		/// <summary>
		/// Converts an angle in degrees to radians as: val*PI/180.
		/// </summary>
		/// <param name="val">A floating point value that represents an angle in degrees.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode ToRadians(ScalarNode val);

		// System.Numerics functions

		/// <summary>
		/// Returns the absolute value of the specified input. For vectors, the absolute value of each subchannel is returned.
		/// </summary>
		/// <param name="val">The input value.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Abs(ScalarNode val);

		/// <summary>
		/// Returns the absolute value of the specified input. For vectors, the absolute value of each subchannel is returned.
		/// </summary>
		/// <param name="val">The input value.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Abs(Vector2Node val);
		/// <summary>
		/// Returns the absolute value of the specified input. For vectors, the absolute value of each subchannel is returned.
		/// </summary>
		/// <param name="val">The input value.</param>
		/// <returns>Vector3Node.</returns>
		static Vector3Node Abs(Vector3Node val);

		/// <summary>
		/// Returns the absolute value of the specified input. For vectors, the absolute value of each subchannel is returned.
		/// .
		/// </summary>
		/// <param name="val">The input value.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Abs(Vector4Node val);

		/// <summary>
		/// Restricts a value to be within a specified range. For vectors, each subchannel is clamped.
		/// </summary>
		/// <param name="val">The value to clamp.</param>
		/// <param name="min">The specified minimum range.</param>
		/// <param name="max">The specified maximum range.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Clamp(ScalarNode val, ScalarNode min, ScalarNode max);
		/// <summary>
		/// Restricts a value to be within a specified range. For vectors, each subchannel is clamped.
		/// </summary>
		/// <param name="val">The value to clamp.</param>
		/// <param name="min">The specified minimum range.</param>
		/// <param name="max">The specified maximum range.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Clamp(Vector2Node val, Vector2Node min, Vector2Node max);

		/// <summary>
		/// Restricts a value to be within a specified range. For vectors, each subchannel is clamped.
		/// </summary>
		/// <param name="val">The value to clamp.</param>
		/// <param name="min">The specified minimum range.</param>
		/// <param name="max">The specified maximum range.</param>
		/// <returns>Vector3Node.</returns>
		static Vector3Node Clamp(Vector3Node val, Vector3Node min, Vector3Node max);

		/// <summary>
		/// Restricts a value to be within a specified range. For vectors, each subchannel is clamped.
		/// </summary>
		/// <param name="val">The value to clamp.</param>
		/// <param name="min">The specified minimum range.</param>
		/// <param name="max">The specified maximum range.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Clamp(Vector4Node val, Vector4Node min, Vector4Node max);

		/// <summary>
		/// Linearly interpolates between two colors in the default color space.
		/// </summary>
		/// <param name="val1">Color source value 1.</param>
		/// <param name="val2">Color source value 2.</param>
		/// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
		/// <returns>ColorNode.</returns>
		static ColorNode ColorLerp(ColorNode val1, ColorNode val2, ScalarNode progress);

		/// <summary>
		/// Linearly interpolates between two colors in the HSL color space.
		/// </summary>
		/// <param name="val1">Color source value 1.</param>
		/// <param name="val2">Color source value 2.</param>
		/// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
		/// <returns>ColorNode.</returns>
		static ColorNode ColorLerpHsl(ColorNode val1, ColorNode val2, ScalarNode progress);

		/// <summary>
		/// Linearly interpolates between two colors in the RBG color space.
		/// </summary>
		/// <param name="val1">Color source value 1.</param>
		/// <param name="val2">Color source value 2.</param>
		/// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
		/// <returns>ColorNode.</returns>
		static ColorNode ColorLerpRgb(ColorNode val1, ColorNode val2, ScalarNode progress);

		/// <summary>
		/// Concatenates two Quaternions; the result represents the first rotation followed by the second rotation.
		/// </summary>
		/// <param name="val1">The first quaternion rotation in the series.</param>
		/// <param name="val2">The second quaternion rotation in the series.</param>
		/// <returns>QuaternionNode.</returns>
		static QuaternionNode Concatenate(QuaternionNode val1, QuaternionNode val2);

		/// <summary>
		/// Returns the distance between two vectors as: sqrt((x1-x2)^2 + (y1-y2)^2 + ...).
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Distance(ScalarNode val1, ScalarNode val2);

		/// <summary>
		/// Returns the distance between two vectors as: sqrt((x1-x2)^2 + (y1-y2)^2 + ...).
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Distance(Vector2Node val1, Vector2Node val2);

		/// <summary>
		/// Returns the distance between two vectors as: sqrt((x1-x2)^2 + (y1-y2)^2 + ...).
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Distance(Vector3Node val1, Vector3Node val2);

		/// <summary>
		/// Returns the distance between two vectors as: sqrt((x1-x2)^2 + (y1-y2)^2 + ...).
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Distance(Vector4Node val1, Vector4Node val2);

		/// <summary>
		/// Returns the squared distance between two vectors as: ((x1-x2)^2 + (y1-y2)^2 + ...).
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode DistanceSquared(ScalarNode val1, ScalarNode val2);

		/// <summary>
		/// Returns the squared distance between two vectors as: ((x1-x2)^2 + (y1-y2)^2 + ...).
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode DistanceSquared(Vector2Node val1, Vector2Node val2);

		/// <summary>
		/// Returns the squared distance between two vectors as: ((x1-x2)^2 + (y1-y2)^2 + ...).
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode DistanceSquared(Vector3Node val1, Vector3Node val2);

		/// <summary>
		/// Returns the squared distance between two vectors as: ((x1-x2)^2 + (y1-y2)^2 + ...).
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode DistanceSquared(Vector4Node val1, Vector4Node val2);

		/// <summary>
		/// Returns the inverse of the specified matrix.
		/// </summary>
		/// <param name="val">The matrix to invert.</param>
		/// <returns>Matrix3x2Node.</returns>
		static Matrix3x2Node Inverse(Matrix3x2Node val);

		/// <summary>
		/// Returns the inverse of the specified matrix.
		/// </summary>
		/// <param name="val">The matrix to invert.</param>
		/// <returns>Matrix4x4Node.</returns>
		static Matrix4x4Node Inverse(Matrix4x4Node val);
		/// <summary>
		/// Returns the length of the vector as: sqrt(x^2 + y^2 + ...).
		/// </summary>
		/// <param name="val">Vector value to return the length of.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Length(ScalarNode val);

		/// <summary>
		/// Returns the length of the vector as: sqrt(x^2 + y^2 + ...).
		/// </summary>
		/// <param name="val">Vector value to return the length of.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Length(Vector2Node val);

		/// <summary>
		/// Returns the length of the vector as: sqrt(x^2 + y^2 + ...).
		/// </summary>
		/// <param name="val">Vector value to return the length of.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Length(Vector3Node val);

		/// <summary>
		/// Returns the length of the vector as: sqrt(x^2 + y^2 + ...).
		/// </summary>
		/// <param name="val">Vector value to return the length of.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Length(Vector4Node val);

		/// <summary>
		/// Returns the length of the vector as: sqrt(x^2 + y^2 + ...).
		/// </summary>
		/// <param name="val">Vector value to return the length of.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Length(QuaternionNode val);

		/// <summary>
		/// Returns the squared length of the vector as: (x^2 + y^2 + ...).
		/// </summary>
		/// <param name="val">Vector value to return the length squared of.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode LengthSquared(ScalarNode val);

		/// <summary>
		/// Returns the squared length of the vector as: (x^2 + y^2 + ...).
		/// </summary>
		/// <param name="val">Vector value to return the length squared of.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode LengthSquared(Vector2Node val);

		/// <summary>
		/// Returns the squared length of the vector as: (x^2 + y^2 + ...).
		/// </summary>
		/// <param name="val">Vector value to return the length squared of.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode LengthSquared(Vector3Node val);

		/// <summary>
		/// Returns the squared length of the vector as: (x^2 + y^2 + ...).
		/// </summary>
		/// <param name="val">Vector value to return the length squared of.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode LengthSquared(Vector4Node val);

		/// <summary>
		/// Returns the squared length of the vector as: (x^2 + y^2 + ...).
		/// </summary>
		/// <param name="val">Vector value to return the length squared of.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode LengthSquared(QuaternionNode val);

		/// <summary>
		/// Linearly interpolates between two vectors as: Output.x = x1 + (x2-x1)*progress.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Lerp(ScalarNode val1, ScalarNode val2, ScalarNode progress);

		/// <summary>
		/// Linearly interpolates between two vectors as: Output.x = x1 + (x2-x1)*progress.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Lerp(Vector2Node val1, Vector2Node val2, ScalarNode progress);

		/// <summary>
		/// Linearly interpolates between two vectors as: Output.x = x1 + (x2-x1)*progress.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
		/// <returns>Vector3Node.</returns>
		static Vector3Node Lerp(Vector3Node val1, Vector3Node val2, ScalarNode progress);

		/// <summary>
		/// Linearly interpolates between two vectors as: Output.x = x1 + (x2-x1)*progress.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Lerp(Vector4Node val1, Vector4Node val2, ScalarNode progress);

		/// <summary>
		/// Returns the maximum of two values. For vectors, the max of each subchannel is returned.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Max(ScalarNode val1, ScalarNode val2);

		/// <summary>
		/// Returns the maximum of two values. For vectors, the max of each subchannel is returned.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Max(Vector2Node val1, Vector2Node val2);

		/// <summary>
		/// Returns the maximum of two values. For vectors, the max of each subchannel is returned.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>Vector3Node.</returns>
		static Vector3Node Max(Vector3Node val1, Vector3Node val2);

		/// <summary>
		/// Returns the maximum of two values. For vectors, the max of each subchannel is returned.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Max(Vector4Node val1, Vector4Node val2);

		/// <summary>
		/// Returns the minimum of two values. For vectors, the min of each subchannel is returned.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Min(ScalarNode val1, ScalarNode val2);

		/// <summary>
		/// Returns the minimum of two values. For vectors, the min of each subchannel is returned.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Min(Vector2Node val1, Vector2Node val2);

		/// <summary>
		/// Returns the minimum of two values. For vectors, the min of each subchannel is returned.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>Vector3Node.</returns>
		static Vector3Node Min(Vector3Node val1, Vector3Node val2);

		/// <summary>
		/// Returns the minimum of two values. For vectors, the min of each subchannel is returned.
		/// </summary>
		/// <param name="val1">Source value 1.</param>
		/// <param name="val2">Source value 2.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Min(Vector4Node val1, Vector4Node val2);

		/// <summary>
		/// Returns the remainder resulting from dividing val1/val2. For vectors, the remainder for each subchannel is returned.
		/// </summary>
		/// <param name="val1">The numerator value.</param>
		/// <param name="val2">The denominator value.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Mod(ScalarNode val1, ScalarNode val2);

		/// <summary>
		/// Returns the remainder resulting from dividing val1/val2. For vectors, the remainder for each subchannel is returned.
		/// </summary>
		/// <param name="val1">The numerator value.</param>
		/// <param name="val2">The denominator value.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Mod(Vector2Node val1, Vector2Node val2);

		/// <summary>
		/// Returns the remainder resulting from dividing val1/val2. For vectors, the remainder for each subchannel is returned.
		/// </summary>
		/// <param name="val1">The numerator value.</param>
		/// <param name="val2">The denominator value.</param>
		/// <returns>Vector3Node.</returns>
		static Vector3Node Mod(Vector3Node val1, Vector3Node val2);

		/// <summary>
		/// Returns the remainder resulting from dividing val1/val2. For vectors, the remainder for each subchannel is returned.
		/// </summary>
		/// <param name="val1">The numerator value.</param>
		/// <param name="val2">The denominator value.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Mod(Vector4Node val1, Vector4Node val2);

		/// <summary>
		/// Returns the normalized version of a vector.
		/// </summary>
		/// <param name="val">Vector value to normalize.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Normalize(Vector2Node val);

		/// <summary>
		/// Returns the normalized version of a vector.
		/// </summary>
		/// <param name="val">Vector value to normalize.</param>
		/// <returns>Vector3Node.</returns>
		static Vector3Node Normalize(Vector3Node val);

		/// <summary>
		/// Returns the normalized version of a vector.
		/// </summary>
		/// <param name="val">Vector value to normalize.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Normalize(Vector4Node val);

		/// <summary>
		/// Returns the normalized version of a vector.
		/// </summary>
		/// <param name="val">Vector value to normalize.</param>
		/// <returns>QuaternionNode.</returns>
		static QuaternionNode Normalize(QuaternionNode val);

		/// <summary>
		/// Multiply each subchannel of the specified vector/matrix by a float value.
		/// </summary>
		/// <param name="val1">Source value to scale.</param>
		/// <param name="val2">Scaling value.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Scale(ScalarNode val1, ScalarNode val2);

		/// <summary>
		/// Multiply each subchannel of the specified vector/matrix by a float value.
		/// </summary>
		/// <param name="val1">Source value to scale.</param>
		/// <param name="val2">Scaling value.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Scale(Vector2Node val1, ScalarNode val2);

		/// <summary>
		/// Multiply each subchannel of the specified vector/matrix by a float value.
		/// </summary>
		/// <param name="val1">Source value to scale.</param>
		/// <param name="val2">Scaling value.</param>
		/// <returns>Vector3Node.</returns>
		static Vector3Node Scale(Vector3Node val1, ScalarNode val2);

		/// <summary>
		/// Multiply each subchannel of the specified vector/matrix by a float value.
		/// </summary>
		/// <param name="val1">Source value to scale.</param>
		/// <param name="val2">Scaling value.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Scale(Vector4Node val1, ScalarNode val2);

		/// <summary>
		/// Multiply each subchannel of the specified vector/matrix by a float value.
		/// </summary>
		/// <param name="val1">Source value to scale.</param>
		/// <param name="val2">Scaling value.</param>
		/// <returns>Matrix3x2Node.</returns>
		static Matrix3x2Node Scale(Matrix3x2Node val1, ScalarNode val2);

		/// <summary>
		/// Multiply each subchannel of the specified vector/matrix by a float value.
		/// </summary>
		/// <param name="val1">Source value to scale.</param>
		/// <param name="val2">Scaling value.</param>
		/// <returns>Matrix4x4Node.</returns>
		static Matrix4x4Node Scale(Matrix4x4Node val1, ScalarNode val2);

		/// <summary>
		/// Spherically interpolates between two quaternions.
		/// </summary>
		/// <param name="val1">Quaternion source value 1.</param>
		/// <param name="val2">Quaternion source value 2.</param>
		/// <param name="progress">A value between 0 and 1.0 indicating the weight of val2.</param>
		/// <returns>QuaternionNode.</returns>
		static QuaternionNode Slerp(QuaternionNode val1, QuaternionNode val2, ScalarNode progress);

		/// <summary>
		/// Transforms a vector by the specified matrix.
		/// </summary>
		/// <param name="val1">Vector to be transformed.</param>
		/// <param name="val2">The transformation matrix.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Transform(Vector2Node val1, Matrix3x2Node val2);

		/// <summary>
		/// Transforms a vector by the specified matrix.
		/// </summary>
		/// <param name="val1">Vector to be transformed.</param>
		/// <param name="val2">The transformation matrix.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Transform(Vector4Node val1, Matrix4x4Node val2);

		// System.Numerics Type Constructors

		/// <summary>
		/// Creates a vector whose subchannels have the specified values.
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Vector2(ScalarNode x, ScalarNode y);

		/// <summary>
		/// Creates a vector whose subchannels have the specified values.
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		/// <param name="z">The z.</param>
		/// <returns>Vector3Node.</returns>
		static Vector3Node Vector3(ScalarNode x, ScalarNode y, ScalarNode z);

		/// <summary>
		/// Creates a vector whose subchannels have the specified values.
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		/// <param name="z">The z.</param>
		/// <param name="w">The w.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Vector4(ScalarNode x, ScalarNode y, ScalarNode z, ScalarNode w);

		/// <summary>
		/// Creates a color in the HSL format.
		/// </summary>
		/// <param name="h">Hue</param>
		/// <param name="s">Saturation</param>
		/// <param name="l">Luminosity</param>
		/// <returns>ColorNode.</returns>
		static ColorNode ColorHsl(ScalarNode h, ScalarNode s, ScalarNode l);

		/// <summary>
		/// Creates a Color in the ARGB format.
		/// </summary>
		/// <param name="alpha">The alpha.</param>
		/// <param name="red">The red.</param>
		/// <param name="green">The green.</param>
		/// <param name="blue">The blue.</param>
		/// <returns>ColorNode.</returns>
		static ColorNode ColorRgb(ScalarNode alpha, ScalarNode red, ScalarNode green, ScalarNode blue);

		/// <summary>
		/// Creates a quaternion whose subchannels have the specified values.
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		/// <param name="z">The z.</param>
		/// <param name="w">The w.</param>
		/// <returns>QuaternionNode.</returns>
		static QuaternionNode Quaternion(ScalarNode x, ScalarNode y, ScalarNode z, ScalarNode w);

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
		static Matrix3x2Node Matrix3x2(ScalarNode channel11, ScalarNode channel12, ScalarNode channel21, ScalarNode channel22, ScalarNode channel31, ScalarNode channel32);

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

		static Matrix4x4Node Matrix4x4(ScalarNode channel11, ScalarNode channel12, ScalarNode channel13, ScalarNode channel14,
			ScalarNode channel21, ScalarNode channel22, ScalarNode channel23, ScalarNode channel24,
			ScalarNode channel31, ScalarNode channel32, ScalarNode channel33, ScalarNode channel34,
			ScalarNode channel41, ScalarNode channel42, ScalarNode channel43, ScalarNode channel44);

		/// <summary>
		/// Creates a 4x4 matrix from a 3x2 matrix.
		/// </summary>
		/// <param name="val">The value.</param>
		/// <returns>Matrix4x4Node.</returns>
		static Matrix4x4Node Matrix4x4(Matrix3x2Node val);

		/// <summary>
		/// Creates a translation matrix from the specified vector.
		/// </summary>
		/// <param name="val">Source translation vector.</param>
		/// <returns>Matrix3x2Node.</returns>
		static Matrix3x2Node CreateTranslation(Vector2Node val);

		/// <summary>
		/// Creates a translation matrix from the specified vector.
		/// </summary>
		/// <param name="val">Source translation vector.</param>
		/// <returns>Matrix4x4Node.</returns>
		static Matrix4x4Node CreateTranslation(Vector3Node val);

		/// <summary>
		/// Creates a scale matrix from the specified vector scale.
		/// </summary>
		/// <param name="val">Source scaling vector.</param>
		/// <returns>Matrix3x2Node.</returns>
		static Matrix3x2Node CreateScale(Vector2Node val);

		/// <summary>
		/// Creates a scale matrix from the specified vector scale.
		/// </summary>
		/// <param name="val">Source scaling vector.</param>
		/// <returns>Matrix4x4Node.</returns>
		static Matrix4x4Node CreateScale(Vector3Node val);

		/// <summary>
		/// Creates a skew matrix from the specified angles in radians.
		/// </summary>
		/// <param name="xAngle">X angle, in radians.</param>
		/// <param name="yAngle">Y angle, in radians.</param>
		/// <param name="centerPoint">The centerpoint for the operation.</param>
		/// <returns>Matrix3x2Node.</returns>
		static Matrix3x2Node CreateSkew(ScalarNode xAngle, ScalarNode yAngle, Vector2Node centerPoint);

		/// <summary>
		/// Creates a rotation matrix using the given rotation in radians.
		/// </summary>
		/// <param name="angle">Angle, in radians.</param>
		/// <returns>Matrix3x2Node.</returns>
		static Matrix3x2Node CreateRotation(ScalarNode angle);

		/// <summary>
		/// Creates a matrix that rotates around an arbitrary vector.
		/// </summary>
		/// <param name="axis">Rotation axis</param>
		/// <param name="angle">Angle, in radians.</param>
		/// <returns>Matrix4x4Node.</returns>
		static Matrix4x4Node CreateMatrix4x4FromAxisAngle(Vector3Node axis, ScalarNode angle);

		/// <summary>
		/// Creates a quaternion that rotates around an arbitrary vector.
		/// </summary>
		/// <param name="axis">Rotation axis</param>
		/// <param name="angle">Angle, in radians.</param>
		/// <returns>QuaternionNode.</returns>
		static QuaternionNode CreateQuaternionFromAxisAngle(Vector3Node axis, ScalarNode angle);

		/// <summary>
		/// Performs a logical AND operation on two boolean values as: val1 &amp;&amp; val2.
		/// </summary>
		/// <param name="val1">The val1.</param>
		/// <param name="val2">The val2.</param>
		/// <returns>BooleanNode.</returns>
		static BooleanNode And(BooleanNode val1, BooleanNode val2);

		/// <summary>
		/// Performs a logical OR operation on two boolean values as: val1 || val2.
		/// </summary>
		/// <param name="val1">The val1.</param>
		/// <param name="val2">The val2.</param>
		/// <returns>BooleanNode.</returns>
		static BooleanNode Or(BooleanNode val1, BooleanNode val2);

		/// <summary>
		/// Performs a logical NOT operation on a specified boolean value as: !val.
		/// </summary>
		/// <param name="val">The value.</param>
		/// <returns>BooleanNode.</returns>
		static BooleanNode Not(BooleanNode val);

		/// <summary>
		/// Returns one of two values, depending on the value of the boolean condition.
		/// </summary>
		/// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
		/// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
		/// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
		/// <returns>ScalarNode.</returns>
		static ScalarNode Conditional(BooleanNode condition, ScalarNode trueCase, ScalarNode falseCase);

		/// <summary>
		/// Returns one of two values, depending on the value of the boolean condition.
		/// </summary>
		/// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
		/// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
		/// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
		/// <returns>Vector2Node.</returns>
		static Vector2Node Conditional(BooleanNode condition, Vector2Node trueCase, Vector2Node falseCase);

		/// <summary>
		/// Returns one of two values, depending on the value of the boolean condition.
		/// </summary>
		/// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
		/// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
		/// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
		/// <returns>Vector3Node.</returns>
		static Vector3Node Conditional(BooleanNode condition, Vector3Node trueCase, Vector3Node falseCase);

		/// <summary>
		/// Returns one of two values, depending on the value of the boolean condition.
		/// </summary>
		/// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
		/// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
		/// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
		/// <returns>Vector4Node.</returns>
		static Vector4Node Conditional(BooleanNode condition, Vector4Node trueCase, Vector4Node falseCase);

		/// <summary>
		/// Returns one of two values, depending on the value of the boolean condition.
		/// </summary>
		/// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
		/// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
		/// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
		/// <returns>ColorNode.</returns>
		static ColorNode Conditional(BooleanNode condition, ColorNode trueCase, ColorNode falseCase);

		/// <summary>
		/// Returns one of two values, depending on the value of the boolean condition.
		/// </summary>
		/// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
		/// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
		/// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
		/// <returns>QuaternionNode.</returns>
		static QuaternionNode Conditional(BooleanNode condition, QuaternionNode trueCase, QuaternionNode falseCase);

		/// <summary>
		/// Returns one of two values, depending on the value of the boolean condition.
		/// </summary>
		/// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
		/// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
		/// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
		/// <returns>Matrix3x2Node.</returns>
		static Matrix3x2Node Conditional(BooleanNode condition, Matrix3x2Node trueCase, Matrix3x2Node falseCase);

		/// <summary>
		/// Returns one of two values, depending on the value of the boolean condition.
		/// </summary>
		/// <param name="condition">Boolean value used to determine whether to return the value represented by 'trueCase' or 'falseCase'.</param>
		/// <param name="trueCase">Value to return if 'condition' evaluates to true.</param>
		/// <param name="falseCase">Value to return if 'condition' evaluates to false.</param>
		/// <returns>Matrix4x4Node.</returns>
		static Matrix4x4Node Conditional(BooleanNode condition, Matrix4x4Node trueCase, Matrix4x4Node falseCase);

		/// <summary>
		/// Functions the specified node type.
		/// </summary>
		/// <typeparam name="T">A class that derives from ExpressionNode.</typeparam>
		/// <param name="nodeType">Type of the node.</param>
		/// <param name="expressionFunctionParams">The expression function parameters.</param>
		/// <returns>T.</returns>
        template<typename T>
        static T Function(ExpressionNodeType nodeType, std::initializer_list<std::reference_wrapper<ExpressionNode const>> expressionFunctionParams)
            requires std::is_base_of_v<ExpressionNode, T>
        {
            T newNode = ExpressionNode::CreateExpressionNode<T>();
            newNode.NodeType = nodeType;

            for (auto const& param : expressionFunctionParams)
            {
                newNode.AddChild(param.get());
            }

            return newNode;
        }

        template<typename T, typename... TArgs>
        static T Function(ExpressionNodeType nodeType, TArgs const&... expressionFunctionParams)
            requires std::is_base_of_v<ExpressionNode, T> && (std::is_base_of_v<ExpressionNode, TArgs> && ...)
        {
            return Function<T>(nodeType, { std::cref(static_cast<ExpressionNode const&>(expressionFunctionParams))... });
        }
		public:
		/// <summary>
		/// Struct ExpressionNodeInfo
		/// </summary>
		struct ExpressionNodeInfo
		{
			/// <summary>
			/// Initializes a new instance of the <see cref="ExpressionNodeInfo"/> struct.
			/// </summary>
			/// <param name="nodeOperationKind">Kind of the node operation.</param>
			/// <param name="operationString">The operation string.</param>
		public:
			ExpressionNodeInfo(OperationType nodeOperationKind, std::optional<winrt::hstring> operationString)
			{
				NodeOperationKind = nodeOperationKind;
				OperationString = operationString;
			}

			/// <summary>
			/// Gets or sets the kind of the node operation.
			/// </summary>
			/// <value>The kind of the node operation.</value>
			OperationType NodeOperationKind{ OperationType::Constant };

			/// <summary>
			/// Gets or sets the operation string.
			/// </summary>
			/// <value>The operation string.</value>
			std::optional<winrt::hstring> OperationString;
		};

		/// <summary>
		/// The expression node information
		/// </summary>
	private:
		static const inline std::map<ExpressionNodeType, ExpressionNodeInfo> _expressionNodeInfo =
		{
			{ ExpressionNodeType::ConstantValue,            ExpressionNodeInfo(OperationType::Constant,      std::nullopt) },
			{ ExpressionNodeType::ConstantParameter,        ExpressionNodeInfo(OperationType::Constant,      std::nullopt) },
			{ ExpressionNodeType::CurrentValueProperty,     ExpressionNodeInfo(OperationType::Reference,     std::nullopt) },
			{ ExpressionNodeType::Reference,                ExpressionNodeInfo(OperationType::Reference,     std::nullopt) },
			{ ExpressionNodeType::ReferenceProperty,        ExpressionNodeInfo(OperationType::Reference,     std::nullopt) },
			{ ExpressionNodeType::StartingValueProperty,    ExpressionNodeInfo(OperationType::Reference,     std::nullopt) },
			{ ExpressionNodeType::TargetReference,          ExpressionNodeInfo(OperationType::Reference,     std::nullopt) },
			{ ExpressionNodeType::Conditional,              ExpressionNodeInfo(OperationType::Conditional,   std::nullopt) },
			{ ExpressionNodeType::Swizzle,                  ExpressionNodeInfo(OperationType::Swizzle,       std::nullopt) },
			{ ExpressionNodeType::Add,                      ExpressionNodeInfo(OperationType::Operator,      L"+") },
			{ ExpressionNodeType::And,                      ExpressionNodeInfo(OperationType::Operator,      L"&&") },
			{ ExpressionNodeType::Divide,                   ExpressionNodeInfo(OperationType::Operator,      L"/") },
			{ ExpressionNodeType::Equals,                   ExpressionNodeInfo(OperationType::Operator,      L"==") },
			{ ExpressionNodeType::GreaterThan,              ExpressionNodeInfo(OperationType::Operator,      L">") },
			{ ExpressionNodeType::GreaterThanEquals,        ExpressionNodeInfo(OperationType::Operator,      L">=") },
			{ ExpressionNodeType::LessThan,                 ExpressionNodeInfo(OperationType::Operator,      L"<") },
			{ ExpressionNodeType::LessThanEquals,           ExpressionNodeInfo(OperationType::Operator,      L"<=") },
			{ ExpressionNodeType::Multiply,                 ExpressionNodeInfo(OperationType::Operator,      L"*") },
			{ ExpressionNodeType::Not,                      ExpressionNodeInfo(OperationType::UnaryOperator, L"!") },
			{ ExpressionNodeType::NotEquals,                ExpressionNodeInfo(OperationType::Operator,      L"!=") },
			{ ExpressionNodeType::Or,                       ExpressionNodeInfo(OperationType::Operator,      L"||") },
			{ ExpressionNodeType::Subtract,                 ExpressionNodeInfo(OperationType::Operator,      L"-") },
			{ ExpressionNodeType::Absolute,                 ExpressionNodeInfo(OperationType::Function,      L"abs") },
			{ ExpressionNodeType::Acos,                     ExpressionNodeInfo(OperationType::Function,      L"acos") },
			{ ExpressionNodeType::Asin,                     ExpressionNodeInfo(OperationType::Function,      L"asin") },
			{ ExpressionNodeType::Atan,                     ExpressionNodeInfo(OperationType::Function,      L"atan") },
			{ ExpressionNodeType::Cos,                      ExpressionNodeInfo(OperationType::Function,      L"cos") },
			{ ExpressionNodeType::Ceil,                     ExpressionNodeInfo(OperationType::Function,      L"ceil") },
			{ ExpressionNodeType::Clamp,                    ExpressionNodeInfo(OperationType::Function,      L"clamp") },
			{ ExpressionNodeType::ColorHsl,                 ExpressionNodeInfo(OperationType::Function,      L"colorhsl") },
			{ ExpressionNodeType::ColorRgb,                 ExpressionNodeInfo(OperationType::Function,      L"colorrgb") },
			{ ExpressionNodeType::ColorLerp,                ExpressionNodeInfo(OperationType::Function,      L"colorlerp") },
			{ ExpressionNodeType::ColorLerpHsl,             ExpressionNodeInfo(OperationType::Function,      L"colorlerphsl") },
			{ ExpressionNodeType::ColorLerpRgb,             ExpressionNodeInfo(OperationType::Function,      L"colorlerprgb") },
			{ ExpressionNodeType::Concatenate,              ExpressionNodeInfo(OperationType::Function,      L"concatenate") },
			{ ExpressionNodeType::Distance,                 ExpressionNodeInfo(OperationType::Function,      L"distance") },
			{ ExpressionNodeType::DistanceSquared,          ExpressionNodeInfo(OperationType::Function,      L"distancesquared") },
			{ ExpressionNodeType::Floor,                    ExpressionNodeInfo(OperationType::Function,      L"floor") },
			{ ExpressionNodeType::Inverse,                  ExpressionNodeInfo(OperationType::Function,      L"inverse") },
			{ ExpressionNodeType::Length,                   ExpressionNodeInfo(OperationType::Function,      L"length") },
			{ ExpressionNodeType::LengthSquared,            ExpressionNodeInfo(OperationType::Function,      L"lengthsquared") },
			{ ExpressionNodeType::Lerp,                     ExpressionNodeInfo(OperationType::Function,      L"lerp") },
			{ ExpressionNodeType::Ln,                       ExpressionNodeInfo(OperationType::Function,      L"ln") },
			{ ExpressionNodeType::Log10,                    ExpressionNodeInfo(OperationType::Function,      L"log10") },
			{ ExpressionNodeType::Max,                      ExpressionNodeInfo(OperationType::Function,      L"max") },
			{ ExpressionNodeType::Matrix3x2FromRotation,    ExpressionNodeInfo(OperationType::Function,      L"matrix3x2.createrotation") },
			{ ExpressionNodeType::Matrix3x2FromScale,       ExpressionNodeInfo(OperationType::Function,      L"matrix3x2.createscale") },
			{ ExpressionNodeType::Matrix3x2FromSkew,        ExpressionNodeInfo(OperationType::Function,      L"matrix3x2.createskew") },
			{ ExpressionNodeType::Matrix3x2FromTranslation, ExpressionNodeInfo(OperationType::Function,      L"matrix3x2.createtranslation") },
			{ ExpressionNodeType::Matrix3x2,                ExpressionNodeInfo(OperationType::Function,      L"matrix3x2") },
			{ ExpressionNodeType::Matrix4x4FromAxisAngle,   ExpressionNodeInfo(OperationType::Function,      L"matrix4x4.createfromaxisangle") },
			{ ExpressionNodeType::Matrix4x4FromScale,       ExpressionNodeInfo(OperationType::Function,      L"matrix4x4.createscale") },
			{ ExpressionNodeType::Matrix4x4FromTranslation, ExpressionNodeInfo(OperationType::Function,      L"matrix4x4.createtranslation") },
			{ ExpressionNodeType::Matrix4x4,                ExpressionNodeInfo(OperationType::Function,      L"matrix4x4") },
			{ ExpressionNodeType::Min,                      ExpressionNodeInfo(OperationType::Function,      L"min") },
			{ ExpressionNodeType::Modulus,                  ExpressionNodeInfo(OperationType::Function,      L"mod") },
			{ ExpressionNodeType::Negate,                   ExpressionNodeInfo(OperationType::Function,      L"-") },
			{ ExpressionNodeType::Normalize,                ExpressionNodeInfo(OperationType::Function,      L"normalize") },
			{ ExpressionNodeType::Pow,                      ExpressionNodeInfo(OperationType::Function,      L"pow") },
			{ ExpressionNodeType::QuaternionFromAxisAngle,  ExpressionNodeInfo(OperationType::Function,      L"quaternion.createfromaxisangle") },
			{ ExpressionNodeType::Quaternion,               ExpressionNodeInfo(OperationType::Function,      L"quaternion") },
			{ ExpressionNodeType::Round,                    ExpressionNodeInfo(OperationType::Function,      L"round") },
			{ ExpressionNodeType::Scale,                    ExpressionNodeInfo(OperationType::Function,      L"scale") },
			{ ExpressionNodeType::Sin,                      ExpressionNodeInfo(OperationType::Function,      L"sin") },
			{ ExpressionNodeType::Slerp,                    ExpressionNodeInfo(OperationType::Function,      L"slerp") },
			{ ExpressionNodeType::Sqrt,                     ExpressionNodeInfo(OperationType::Function,      L"sqrt") },
			{ ExpressionNodeType::Square,                   ExpressionNodeInfo(OperationType::Function,      L"square") },
			{ ExpressionNodeType::Tan,                      ExpressionNodeInfo(OperationType::Function,      L"tan") },
			{ ExpressionNodeType::ToDegrees,                ExpressionNodeInfo(OperationType::Function,      L"todegrees") },
			{ ExpressionNodeType::ToRadians,                ExpressionNodeInfo(OperationType::Function,      L"toradians") },
			{ ExpressionNodeType::Transform,                ExpressionNodeInfo(OperationType::Function,      L"transform") },
			{ ExpressionNodeType::Vector2,                  ExpressionNodeInfo(OperationType::Function,      L"vector2") },
			{ ExpressionNodeType::Vector3,                  ExpressionNodeInfo(OperationType::Function,      L"vector3") },
			{ ExpressionNodeType::Vector4,                  ExpressionNodeInfo(OperationType::Function,      L"vector4") },
		};
		public:
		/// <summary>
		/// Gets the type of the node information from.
		/// </summary>
		/// <param name="type">The type.</param>
		/// <returns>ExpressionNodeInfo.</returns>
		static ExpressionNodeInfo GetNodeInfoFromType(ExpressionNodeType type);
	};
}
