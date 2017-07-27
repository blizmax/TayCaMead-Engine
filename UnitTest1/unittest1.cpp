
#include "stdafx.h"
#include "CppUnitTest.h"

#pragma comment(lib, "MathLib/MathLib.lib")

#include "MathLib\Color.h"
#include "MathLib\Vec3.h"
#include "MathLib\Vec4.h"
#include "MathLib\Vec2.h"
#include "MathLib\Mat3.h"
#include "MathLib\Mat4.h"
#include "MathLib\MathUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {

			template<>
			static std::wstring ToString<isart::Mat4>(const isart::Mat4  &mat)
			{
				std::wstring result;
				const size_t matValueSize = 16;
				for (int index = 0; index < matValueSize; ++index)
				{
					result += std::to_wstring(mat.GetRawValueAt(index));
					result += (index == matValueSize ? L"" : L", ");
				}
				return result;
			}

			template<>
			static std::wstring ToString<isart::Mat3>(const isart::Mat3  &mat)
			{
				std::wstring result;
				const size_t matValueSize = 9;
				for (int index = 0; index < matValueSize; ++index)
				{
					result += std::to_wstring(mat.m_values[index]);
					result += (index == matValueSize ? L"" : L", ");
				}
				return result;
			}

			template<>
			static std::wstring ToString<isart::Vec2>(const isart::Vec2  &vec)
			{
				std::wstring result;
				result += std::to_wstring(vec.x);
				result += L", ";
				result += std::to_wstring(vec.y);
				return result;
			}

			template<>
			static std::wstring ToString<isart::Vec4>(const isart::Vec4  &vec)
			{
				std::wstring result;
				result += std::to_wstring(vec.x);
				result += L", ";
				result += std::to_wstring(vec.y);
				result += L", ";
				result += std::to_wstring(vec.z);
				result += L", ";
				result += std::to_wstring(vec.w);
				return result;
			}

			template<>
			static std::wstring ToString<isart::Color>(const isart::Color  &color) 
			{
				std::wstring result;
				result += std::to_wstring(color.r);
				result += L", ";
				result += std::to_wstring(color.g);
				result += L", ";
				result += std::to_wstring(color.b);
				result += L", ";
				result += std::to_wstring(color.a);
				return result;
			}
			template<>
			static std::wstring ToString<isart::Vec3>(const isart::Vec3  &vec)
			{
				std::wstring result;
				result += std::to_wstring(vec.x);
				result += L", ";
				result += std::to_wstring(vec.y);
				result += L", ";
				result += std::to_wstring(vec.z);
				return result;
			}


		}
	}
}

namespace MathLibTest
{
#pragma region Color
	TEST_CLASS(Color)
	{
	public:
		
		TEST_METHOD(Color_Red)
		{
			Assert::AreEqual<isart::Color>(isart::Color(1.f, 0.f, 0.f, 1.f), isart::Color::Red);
		}

		TEST_METHOD(Color_Blue)
		{
			Assert::AreEqual<isart::Color>(isart::Color(0.f, 0.f, 1.f, 1.f), isart::Color::Blue);
		}

		TEST_METHOD(Color_Green)
		{
			Assert::AreEqual<isart::Color>(isart::Color(0.f, 1.f, 0.f, 1.f), isart::Color::Green);
		}

		TEST_METHOD(Color_Constructor_By_Copy)
		{
			isart::Color a = isart::Color::Red;
			isart::Color b(a);
			Assert::AreEqual<isart::Color>(a, b);
		}

		TEST_METHOD(Color_With_Vec3)
		{
			isart::Vec3 b(1.f, 1.f, 1.f);
			isart::Color a(b);
			Assert::AreEqual<isart::Color>(a, isart::Color::White);
		}

		TEST_METHOD(Color_With_Vec4)
		{
			isart::Vec4 b(1.f, 1.f, 1.f, 1.f);
			isart::Color a(b);
			Assert::AreEqual<isart::Color>(a, isart::Color::White);
		}

		TEST_METHOD(Color_Operator_plus)
		{
			isart::Color a(0.5f, 0.5f, 0.5f, 1.f);
			isart::Color b(a);
			Assert::AreEqual<isart::Color>(a+b, isart::Color::White);
		}

		TEST_METHOD(Color_Operator_equal)
		{
			isart::Color a(0.5f, 0.5f, 0.5f, 1.f);
			isart::Color b(a);
			Assert::IsTrue(a == b);
		}

		TEST_METHOD(Color_FromHexRGB)
		{
			// TODO: FromHexRGB will fail with more than 0xFFFFFF
			Assert::AreEqual<isart::Color>(isart::Color::FromHexRGB(0xFF0000), isart::Color::Red);
		}

		TEST_METHOD(Color_FromHexRGBA)
		{
			// TODO: FromHexRGB will fail with more than 0xFFFFFFFF
			Assert::AreEqual<isart::Color>(isart::Color::FromHexRGBA(0xFF0000FF), isart::Color::Red);
		}

		TEST_METHOD(Color_ToGLVec3)
		{
			isart::Color a(0.f, 0.f, 0.f);
			Assert::AreEqual<isart::Color>(a.ToGLVec3(), isart::Vec3());
		}

		TEST_METHOD(Color_ToGLVec4)
		{
			isart::Color a(0.f, 0.f, 0.f, 0.f);
			Assert::AreEqual<isart::Color>(a.ToGLVec4(), isart::Vec4());
		}
	};
#pragma endregion

#pragma region Vec2
	TEST_CLASS(Vec2)
	{
	public:
		TEST_METHOD(Constructor)
		{
			isart::Vec2 a;
			Assert::AreEqual(a, isart::Vec2());
		}

		TEST_METHOD(Constructor_Copy)
		{
			isart::Vec2 a(9.0f, 5.f);
			isart::Vec2 b(a);
			Assert::IsTrue(b == a);
		}

		TEST_METHOD(Operator_multiplie)
		{
			isart::Vec2 a(1.f, 1.f);
			isart::Vec2 b = a * 5.f;
			Assert::AreEqual(b, isart::Vec2(5.f, 5.f));
		}

		TEST_METHOD(Operator_plus)
		{
			isart::Vec2 a(1.f, 1.f);
			isart::Vec2 b = a + isart::Vec2(5.f, 5.f);
			Assert::AreEqual(b, isart::Vec2(6.f, 6.f));
		}

		TEST_METHOD(Operator_minus)
		{
			isart::Vec2 a(1.f, 1.f);
			isart::Vec2 b = a - isart::Vec2(5.f, 5.f);
			Assert::AreEqual(b, isart::Vec2(-4.f, -4.f));
		}

		TEST_METHOD(Operator_equal)
		{
			isart::Vec2 a(1.f, 1.f);
			Assert::IsTrue(a == isart::Vec2(1.f, 1.f));
		}

		TEST_METHOD(Operator_different)
		{
			isart::Vec2 a(1.f, 1.f);
			Assert::IsTrue(a != isart::Vec2(5.f, 1.f));
		}

		TEST_METHOD(Operator_adding_to)
		{
			isart::Vec2 a(1.f, 1.f);
			a += isart::Vec2(2.f, 2.f);
			Assert::AreEqual(a, isart::Vec2(3.f, 3.f));
		}

		TEST_METHOD(Operator_substracting_to)
		{
			isart::Vec2 a(1.f, 1.f);
			a -= isart::Vec2(2.f, 2.f);
			Assert::AreEqual(a, isart::Vec2(-1.f, -1.f));
		}

		TEST_METHOD(Operator_multiplying_to)
		{
			isart::Vec2 a(6.f, 5.f);
			a *= 5.f;
			Assert::AreEqual(a, isart::Vec2(30.f, 25.f));
		}

		TEST_METHOD(Norm)
		{
			isart::Vec2 a(6.f, 5.f);

			Assert::AreEqual(a.Norm(), sqrt((6.f*6.f) + (5.f*5.f)));
		}

		TEST_METHOD(Normalize)
		{
			isart::Vec2 result(0.2f, 0.2f);
			isart::Vec2 trueResult(0.707107f, 0.707107f);
			float epsilon(0.00001f);
			result.Normalize();
			Assert::IsTrue(result.Normalized().Norm() < trueResult.Norm() + epsilon &&
				result.Normalized().Norm() > trueResult.Norm() - epsilon);
		}

		TEST_METHOD(Normalized)
		{
			isart::Vec2 result(0.2f, 0.2f);
			isart::Vec2 trueResult(0.707107f, 0.707107f);
			float epsilon(0.00001f);
			Assert::IsTrue(result.Normalized().Norm() < trueResult.Norm() + epsilon &&
				result.Normalized().Norm() > trueResult.Norm() - epsilon);
		}

		TEST_METHOD(Lerp)
		{
			isart::Vec2 a(0.0f, 0.0f);
			isart::Vec2 b(1.f, 1.f);
			Assert::AreEqual(isart::Vec2::Lerp(a, b, 0.5f), isart::Vec2(0.5f, 0.5f));
		}

		TEST_METHOD(Scalar_Product)
		{
			isart::Vec2 a(0.5f, 0.6f);
			isart::Vec2 b(1.2f, 1.3f);
			float result = isart::Vec2::ScalarProduct(a, b);
			float trueResult = 1.38f;
			float epsilon(0.00001f);
			Assert::IsTrue(result < trueResult + epsilon &&
				result > trueResult - epsilon);
		}

		TEST_METHOD(Distance_To)
		{
			isart::Vec2 a(0.5f, 0.6f);
			isart::Vec2 b(1.2f, 1.3f);
			float result = a.DistanceTo(b);
			float trueResult = 0.989949f;
			float epsilon(0.00001f);
			Assert::IsTrue(result < trueResult + epsilon &&
				result > trueResult - epsilon);
		}

		TEST_METHOD(Is_Zero)
		{
			isart::Vec2 a;
			Assert::IsTrue(a.IsZero());
		}

	};
#pragma endregion

#pragma region Vec3
	TEST_CLASS(Vec3)
	{
	public:
		TEST_METHOD(Constructor)
		{
			isart::Vec3 a;
			Assert::AreEqual(a, isart::Vec3());
		}
		
		TEST_METHOD(Constructor_Copy)
		{
			isart::Vec3 a(9.0f, 5.f, 4.f);
			isart::Vec3 b(a);
			Assert::IsTrue(b == a);
		}

		TEST_METHOD(Operator_multiplie)
		{
			isart::Vec3 a(1.f, 1.f, 1.f);
			isart::Vec3 b = a * 5.f;
			Assert::AreEqual(b, isart::Vec3(5.f, 5.f, 5.f));
		}

		TEST_METHOD(Operator_plus)
		{
			isart::Vec3 a(1.f, 1.f, 1.f);
			isart::Vec3 b = a + isart::Vec3(5.f, 5.f, 5.f);
			Assert::AreEqual(b, isart::Vec3(6.f, 6.f, 6.f));
		}

		TEST_METHOD(Operator_minus)
		{
			isart::Vec3 a(1.f, 1.f, 1.f);
			isart::Vec3 b = a - isart::Vec3(5.f, 5.f, 5.f);
			Assert::AreEqual(b, isart::Vec3(-4.f, -4.f, -4.f));
		}

		TEST_METHOD(Operator_equal)
		{
			isart::Vec3 a(1.f, 4.99999f, 1.f);
			Assert::AreNotEqual(a, isart::Vec3(1.f, 5.000001f, 1.f));
		}

		TEST_METHOD(Operator_different)
		{
			isart::Vec3 a(1.f, 1.f, 1.f);
			Assert::IsTrue(a != isart::Vec3(5.f, 1.f, 1.f));
		}

		TEST_METHOD(Operator_adding_to)
		{
			isart::Vec3 a(1.f, 1.f, 1.f);
			a += isart::Vec3(2.f, 2.f, 2.f);
			Assert::AreEqual(a, isart::Vec3(3.f, 3.f, 3.f));
		}

		TEST_METHOD(Operator_substracting_to)
		{
			isart::Vec3 a(1.f, 1.f, 1.f);
			a -= isart::Vec3(2.f, 2.f, 2.f);
			Assert::AreEqual(a, isart::Vec3(-1.f, -1.f, -1.f));
		}

		TEST_METHOD(Operator_multiplying_to)
		{
			isart::Vec3 a(6.f, 5.f, 4.f);
			a *= 5.f;
			Assert::AreEqual(a, isart::Vec3(30.f, 25.f, 20.f));
		}

		TEST_METHOD(Norm)
		{
			isart::Vec3 a(6.f, 5.f, 4.f);

			Assert::AreEqual(a.Norm(), sqrt((6.f*6.f) + (5.f*5.f) + (4.f*4.f)));
		}

		TEST_METHOD(Normalize)
		{
			isart::Vec3 result(0.2f, 0.2f, 0.2f);
			isart::Vec3 trueResult(0.577350f, 0.577350f, 0.577350f);
			float epsilon(0.00001f);
			result.Normalize();
			Assert::IsTrue(result.Normalized().Norm() < trueResult.Norm() + epsilon &&
				result.Normalized().Norm() > trueResult.Norm() - epsilon);
		}

		TEST_METHOD(Normalized)
		{
			isart::Vec3 result(0.2f, 0.2f, 0.2f);
			isart::Vec3 trueResult(0.577350f, 0.577350f, 0.577350f);
			float epsilon(0.00001f);
			Assert::IsTrue(result.Normalized().Norm() < trueResult.Norm() + epsilon &&
				result.Normalized().Norm() > trueResult.Norm() - epsilon);
		}

		TEST_METHOD(Lerp)
		{
			isart::Vec3 a(0.0f, 0.0f, 0.0f);
			isart::Vec3 b(1.f, 1.f, 1.f);
			Assert::AreEqual(isart::Vec3::Lerp(a, b, 0.5f), isart::Vec3(0.5f, 0.5f, 0.5f));
		}

		TEST_METHOD(Scalar_Product)
		{
			isart::Vec3 a(0.5f, 0.6f, 0.7f);
			isart::Vec3 b(1.2f, 1.3f, 1.4f);
			float result = isart::Vec3::ScalarProduct(a, b);
			float trueResult = 2.36f;
			float epsilon(0.00001f);
			Assert::IsTrue(result < trueResult + epsilon &&
				result > trueResult - epsilon);
		}

		TEST_METHOD(Cross_Product)
		{
			isart::Vec3 a(0.5f, 0.6f, 0.7f);
			isart::Vec3 b(1.2f, 1.3f, 1.4f);
			isart::Vec3 result = isart::Vec3::CrossProduct(a, b);
			isart::Vec3 trueResult = isart::Vec3(-0.07f, 0.14f, -0.07f);
			float epsilon(0.00001f);
			Assert::IsTrue(	result.Norm() < trueResult.Norm() + epsilon &&
							result.Norm() > trueResult.Norm() - epsilon);
		}

		TEST_METHOD(Distance_To)
		{
			isart::Vec3 a(0.5f, 0.6f, 0.7f);
			isart::Vec3 b(1.2f, 1.3f, 1.4f);
			float result = a.DistanceTo(b);
			float trueResult = 1.21244f;
			float epsilon(0.00001f);
			Assert::IsTrue(result < trueResult + epsilon &&
				result > trueResult - epsilon);
		}

		TEST_METHOD(Is_Zero)
		{
			isart::Vec3 a;
			Assert::IsTrue(a.IsZero());
		}

	};
#pragma endregion

#pragma region Vec4
	TEST_CLASS(Vec4)
	{
	public:
		TEST_METHOD(Constructor)
		{
			isart::Vec4 a;
			Assert::AreEqual(a, isart::Vec4());
		}

		TEST_METHOD(Constructor_Copy)
		{
			isart::Vec4 a(9.0f, 5.f, 4.f, 10.f);
			isart::Vec4 b(a);
			Assert::IsTrue(b == a);
		}

		TEST_METHOD(Operator_multiplie)
		{
			isart::Vec4 a(1.f, 1.f, 1.f, 1.f);
			isart::Vec4 b = a * 5.f;
			Assert::AreEqual(b, isart::Vec4(5.f, 5.f, 5.f, 5.f));
		}

		TEST_METHOD(Operator_plus)
		{
			isart::Vec4 a(1.f, 1.f, 1.f, 1.f);
			isart::Vec4 b = a + isart::Vec4(5.f, 5.f, 5.f, 5.f);
			Assert::AreEqual(b, isart::Vec4(6.f, 6.f, 6.f, 6.f));
		}

		TEST_METHOD(Operator_minus)
		{
			isart::Vec4 a(1.f, 1.f, 1.f, 1.f);
			isart::Vec4 b = a - isart::Vec4(5.f, 5.f, 5.f, 5.f);
			Assert::AreEqual(b, isart::Vec4(-4.f, -4.f, -4.f, -4.f));
		}

		TEST_METHOD(Operator_equal)
		{
			isart::Vec4 a(1.f, 4.99999f, 1.f, 1.f);
			Assert::AreNotEqual(a, isart::Vec4(1.f, 5.00001f, 1.f, 1.f));
		}

		TEST_METHOD(Operator_different)
		{
			isart::Vec4 a(1.f, 1.f, 1.f, 5.f);
			Assert::IsTrue(a != isart::Vec4(1.f, 1.f, 1.f, 1.f));
		}

		TEST_METHOD(Operator_adding_to)
		{
			isart::Vec4 a(1.f, 1.f, 1.f, 1.f);
			a += isart::Vec4(2.f, 2.f, 2.f, 2.f);
			Assert::AreEqual(a, isart::Vec4(3.f, 3.f, 3.f, 3.f));
		}

		TEST_METHOD(Operator_substracting_to)
		{
			isart::Vec4 a(1.f, 1.f, 1.f, 1.f);
			a -= isart::Vec4(2.f, 2.f, 2.f, 2.f);
			Assert::AreEqual(a, isart::Vec4(-1.f, -1.f, -1.f, -1.f));
		}

		TEST_METHOD(Operator_multiplying_to)
		{
			isart::Vec4 a(6.f, 5.f, 4.f, 3.f);
			a *= 5.f;
			Assert::AreEqual(a, isart::Vec4(30.f, 25.f, 20.f, 15.f));
		}

		TEST_METHOD(Norm)
		{
			isart::Vec4 a(6.f, 5.f, 4.f, 3.f);

			Assert::AreEqual(a.Norm(), sqrt((6.f*6.f) + (5.f*5.f) + (4.f*4.f) + (3.f*3.f)));
		}

		TEST_METHOD(Normalize)
		{
			isart::Vec4 result(0.2f, 0.2f, 0.2f, 0.2f);
			isart::Vec4 trueResult(0.5f, 0.5f, 0.5f, 0.5f);
			float epsilon(0.00001f);
			result.Normalize();
			Assert::IsTrue(result.Norm() < trueResult.Norm() + epsilon &&
				result.Norm() > trueResult.Norm() - epsilon);
		}

		TEST_METHOD(Normalized)
		{
			isart::Vec4 result(0.2f, 0.2f, 0.2f, 0.2f);
			isart::Vec4 trueResult(0.5f, 0.5f, 0.5f, 0.5f);
			float epsilon(0.00001f);
			Assert::IsTrue(result.Normalized().Norm() < trueResult.Norm() + epsilon &&
				result.Normalized().Norm() > trueResult.Norm() - epsilon);
		}

		TEST_METHOD(Lerp)
		{
			isart::Vec4 a(0.0f, 0.0f, 0.0f, 0.0f);
			isart::Vec4 b(1.f, 1.f, 1.f, 1.f);
			Assert::AreEqual(isart::Vec4::Lerp(a, b, 0.5f), isart::Vec4(0.5f, 0.5f, 0.5f, 0.5f));
		}

		TEST_METHOD(Scalar_Product)
		{
			isart::Vec4 a(0.5f, 0.6f, 0.7f, 0.8f);
			isart::Vec4 b(1.2f, 1.3f, 1.4f, 1.5f);
			float result = isart::Vec4::ScalarProduct(a, b);
			float trueResult = 3.56f;
			float epsilon(0.00001f);
			Assert::IsTrue(result < trueResult + epsilon &&
				result > trueResult - epsilon);
		}

		TEST_METHOD(Distance_To)
		{
			isart::Vec4 a(0.5f, 0.6f, 0.7f, 0.8f);
			isart::Vec4 b(1.2f, 1.3f, 1.4f, 1.5f);
			float result = a.DistanceTo(b);
			float trueResult = 1.4f;
			float epsilon(0.00001f);
			Assert::IsTrue(result < trueResult + epsilon &&
				result > trueResult - epsilon);
		}

		TEST_METHOD(Is_Zero)
		{
			isart::Vec4 a;
			Assert::IsTrue(a.IsZero());
		}

	};
#pragma endregion

#pragma region Mat3
	TEST_CLASS(Mat3)
	{
	public:

		TEST_METHOD(Constructor)
		{
			isart::Mat3 a;
			Assert::AreEqual(isart::Mat3(), a);
		}

		TEST_METHOD(Constructor_By_Copy)
		{
			isart::Mat3 a;
			isart::Mat3 b(a);
			Assert::AreEqual(b, a);
		}

		TEST_METHOD(Constructor_By_Float_Tab)
		{
			float tab[9] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f };
			isart::Mat3 a(tab);
			isart::Mat3 b(tab);
			Assert::AreEqual(b, a);
		}

		TEST_METHOD(Tranpose)
		{
			float tabResult[9] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f };
			isart::Mat3 result(tabResult);
			result.Transpose();

			float trueTabResult[9] = { 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 2.f, 2.f, 2.f };
			isart::Mat3 trueResult(trueTabResult);
			
			
			Assert::AreEqual(trueResult, result);
		}

		TEST_METHOD(Tranposed)
		{
			float tabResult[9] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f };
			isart::Mat3 result(tabResult);

			float trueTabResult[9] = { 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 2.f, 2.f, 2.f };
			isart::Mat3 trueResult(trueTabResult);


			Assert::AreEqual(trueResult, result.Transposed());
		}

		TEST_METHOD(Operator_multiply)
		{
			float tabResult[9] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f };
			isart::Mat3 result(tabResult);

			float trueTabResult[9] = { 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f, 5.f };
			isart::Mat3 trueResult(trueTabResult);


			Assert::AreEqual(trueResult, result * result.Transposed());
		}

		TEST_METHOD(Operator_equal)
		{
			// finish test
			isart::Mat3 result(isart::Mat3::Identity());

			isart::Mat3 trueResult(result);


			Assert::IsTrue(trueResult == result);
		}

		TEST_METHOD(Operator_not_equal)
		{
			// finish test
			isart::Mat3 result(isart::Mat3::Identity());

			float trueTabResult[9] = { 5.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
			isart::Mat3 trueResult(trueTabResult);

			Assert::IsTrue(trueResult != result);
		}

		TEST_METHOD(Identity)
		{
			// finish test
			isart::Mat3 result(isart::Mat3::Identity());

			float trueTabResult[9] = { 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
			isart::Mat3 trueResult(trueTabResult);


			Assert::AreEqual(trueResult, result);
		}

	};
#pragma endregion

#pragma region Mat4
	TEST_CLASS(Mat4)
	{
	public:

		TEST_METHOD(Constructor)
		{
			isart::Mat4 a;
			Assert::AreEqual(isart::Mat4(), a);
		}

		TEST_METHOD(Constructor_With_16_Float_Values_FFS)
		{
			// FFS I hate you guys
			isart::Mat4 a(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);

			Assert::AreEqual(a, isart::Mat4(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f));
		}

		TEST_METHOD(Constructor_By_Copy)
		{
			isart::Mat4 a;
			isart::Mat4 b(a);

			Assert::IsTrue(a == b);
		}

		TEST_METHOD(Constructor_By_Float_Tab)
		{
			float tab[16] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 18.f };
			isart::Mat4 a(tab);
			isart::Mat4 b(tab);
			Assert::AreEqual(b, a);
		}

		TEST_METHOD(Invert)
		{
			float tabResult[16] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 18.f };
			isart::Mat4 result(tabResult);
			result.Invert();

			float trueTabResult[16] = { -3, 2, 8, -1, 2, 8, -4, 0, 8, -4, 2, 0, -1, 0, 0, 1 };
			isart::Mat4 trueResult(trueTabResult);
			trueResult *= 1.f / 18.f;


			Assert::AreEqual(trueResult, result);
		}

		TEST_METHOD(Inverted)
		{
			float tabResult[16] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 18.f };
			isart::Mat4 result(tabResult);

			float trueTabResult[16] = { -3, 2, 8, -1, 2, 8, -4, 0, 8, -4, 2, 0, -1, 0, 0, 1 };
			isart::Mat4 trueResult(trueTabResult);
			trueResult *= 1.f / 18.f;


			Assert::AreEqual(trueResult, result.Inverted());
		}

		TEST_METHOD(Transpose)
		{
			float tabResult[16] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 18.f };
			isart::Mat4 result(tabResult);
			result.Transpose();

			float trueTabResult[16] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 18.f };
			isart::Mat4 trueResult(trueTabResult);

			Assert::AreEqual(trueResult, result);
		}

		TEST_METHOD(Transposed)
		{
			float tabResult[16] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 18.f };
			isart::Mat4 result(tabResult);

			float trueTabResult[16] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 18.f };
			isart::Mat4 trueResult(trueTabResult);

			Assert::AreEqual(trueResult, result.Transposed());
		}

		TEST_METHOD(Get_Vec3_Translation)
		{
			float tabResult[16] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 18.f };
			isart::Mat4 result(tabResult);

			isart::Vec3 trueResult = isart::Mat4::CreateVec3TranslationFromMatrix(result);

			Assert::AreEqual(trueResult, isart::Vec3(0.f, 1.f, 2.f));
		}

		TEST_METHOD(Is_Identity)
		{
			isart::Mat4 trueResult = isart::Mat4::Identity;

			Assert::AreEqual(trueResult, isart::Mat4::Identity);
		}

		TEST_METHOD(Is_Orthogonal)
		{
			isart::Mat4 result(isart::Mat4::Identity);

			Assert::IsTrue(result.IsOrthogonal());
		}

		TEST_METHOD(Get_Raw_Values)
		{
			float tabResult[16] = { -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f };
			isart::Mat4 result(tabResult);

			const size_t maxSize = 16;
			for (size_t index = 0; index < maxSize; ++index)
				Assert::AreEqual(result.GetRawValues()[index], tabResult[index]);
		}

		TEST_METHOD(Get_Raw_Value_At)
		{
			float tabResult[16] = { -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f };
			isart::Mat4 result(tabResult);

			const size_t maxSize = 16;
			for (size_t index = 0; index < maxSize; ++index)
				Assert::AreEqual(result.GetRawValueAt(index), tabResult[index]);
		}

		TEST_METHOD(Get_Row)
		{
			float tabResult[16] = { -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f };
			isart::Mat4 result(tabResult);
			isart::Vec4 result2 = result.GetRow(1);

			Assert::AreEqual(result2, isart::Vec4(1.f, -1.f, 1.f, 1.f));
		}

		TEST_METHOD(Get_Column)
		{
			float tabResult[16] = { -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f };
			isart::Mat4 result(tabResult);
			isart::Vec4 result2 = result.GetColumn(2);

			Assert::AreEqual(result2, isart::Vec4(1.f, 1.f, -1.f, 1.f));
		}

		TEST_METHOD(Operator_Multiply_Mat)
		{
			float tabResult[16] = { -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f };
			isart::Mat4 result(tabResult);
			isart::Mat4 result2(tabResult);
			isart::Mat4 trueResult(4.f, 0.f, 0.f, 0.f, 0.f, 4.f, 0.f, 0.f, 0.f, 0.f, 4.f, 0.f, 0.f, 0.f, 0.f, 4.f);


			Assert::AreEqual(result2 * result, trueResult);
		}

		/* consider *= operator to be ok if this test's Ok */
		TEST_METHOD(Operator_Multiply_Float)
		{
			float tabResult[16] = { 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 0.f, 1.f, 2.f, 18.f };
			isart::Mat4 result(tabResult);

			float trueTabResult[16] = { -3, 2, 8, -1, 2, 8, -4, 0, 8, -4, 2, 0, -1, 0, 0, 1 };
			isart::Mat4 trueResult(trueTabResult);

			Assert::AreEqual(trueResult * (1.f/18.f), result.Inverted());
		}

		TEST_METHOD(Operator_Multiply_Vec4)
		{
			float tabResult[16] = { -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f };
			isart::Mat4 result(tabResult);

			isart::Vec4 trueResult(1.f, 1.f, 1.f, 1.f);

			Assert::AreEqual(result * trueResult, isart::Vec4(2.f ,2.f ,2.f ,2.f ));
		}

		TEST_METHOD(Operator_Multiply_Vec3)
		{
			float tabResult[16] = { -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f };
			isart::Mat4 result(tabResult);

			isart::Vec3 trueResult(1.f, 1.f, 1.f);

			Assert::AreEqual(result * trueResult, isart::Vec3(2.f, 2.f, 2.f));
		}

		TEST_METHOD(Operator_Equal)
		{
			float tabResult[16] = { -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f };
			isart::Mat4 result(tabResult);

			isart::Mat4 trueResult(tabResult);

			Assert::IsTrue(result == trueResult);
		}

		TEST_METHOD(Operator_Not_Equal)
		{
			float tabResult[16] = { -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f };
			isart::Mat4 result(tabResult);

			isart::Mat4 trueResult(tabResult);

			Assert::IsFalse(result != trueResult);
		}

		TEST_METHOD(Rotate_Z)
		{
			float angle = utils::ToRadian(90);
			float tabResult[16] = { cosf(angle), -sinf(angle), 0, 0,
									sinf(angle),  cosf(angle), 0, 0,
									0,          0, 1, 0,
									0,          0, 0, 1, };
			isart::Mat4 result(tabResult);
			isart::Mat4 trueResult = isart::Mat4::Identity;
			trueResult = trueResult.CreateRotZ(90);


			const size_t maxSize = 16;
			for (size_t index = 0; index < maxSize; ++index)
				Assert::AreEqual(result.GetRawValueAt(index), trueResult.GetRawValueAt(index));
		}

		TEST_METHOD(Rotate_Y)
		{
			float angle = utils::ToRadian(90);
			float tabResult[16] = { cosf(angle), 0, -sinf(angle), 0,
									0, 1,          0, 0,
									sinf(angle), 0,  cosf(angle), 0,
									0, 0,          0, 1, };
			isart::Mat4 result(tabResult);
			isart::Mat4 trueResult = isart::Mat4::Identity;
			trueResult = trueResult.CreateRotY(90);


			const size_t maxSize = 16;
			for (size_t index = 0; index < maxSize; ++index)
				Assert::AreEqual(result.GetRawValueAt(index), trueResult.GetRawValueAt(index));
		}

		TEST_METHOD(Rotate_X)
		{
			float angle = utils::ToRadian(90);
			float tabResult[16] = { 1,         0,          0, 0,
									0, cosf(angle), -sinf(angle), 0,
									0, sinf(angle),  cosf(angle), 0,
									0,         0,          0, 1, };
			isart::Mat4 result(tabResult);
			isart::Mat4 trueResult = isart::Mat4::Identity;
			trueResult = trueResult.CreateRotX(90);


			const size_t maxSize = 16;
			for (size_t index = 0; index < maxSize; ++index)
				Assert::AreEqual(result.GetRawValueAt(index), trueResult.GetRawValueAt(index));
		}

		TEST_METHOD(Translation)
		{
			float x, y, z;
			x = y = z = 5.f;

			float tabResult[16] = { 1, 0, 0, x,
									0, 1, 0, y,
									0, 0, 1, z,
									0, 0, 0, 1, };
			isart::Mat4 result(tabResult);
			isart::Mat4 trueResult = isart::Mat4::Identity;
			trueResult = trueResult.CreateTranslation(5.f, 5.f, 5.f);


			const size_t maxSize = 16;
			for (size_t index = 0; index < maxSize; ++index)
				Assert::AreEqual(result.GetRawValueAt(index), trueResult.GetRawValueAt(index));
		}

		TEST_METHOD(Scale)
		{
			float x, y, z;
			x = y = z = 5.f;

			float tabResult[16] = { x, 0, 0, 0,
									0, y, 0, 0,
									0, 0, z, 0,
									0, 0, 0, 1, };
			isart::Mat4 result(tabResult);
			isart::Mat4 trueResult = isart::Mat4::Identity;
			trueResult = trueResult.CreateScale(5.f, 5.f, 5.f);


			const size_t maxSize = 16;
			for (size_t index = 0; index < maxSize; ++index)
				Assert::AreEqual(result.GetRawValueAt(index), trueResult.GetRawValueAt(index));
		}

		TEST_METHOD(Transform)
		{
			float	tx, ty, tz,
				sx, sy, sz,
				rx, ry, rz;
			tx = ty = tz = sx = sy = sz = rx = ry = rz = 5.f;

			
			isart::Mat4 result = isart::Mat4::Identity;
			result = result * isart::Mat4::CreateScale(sx, sy, sz);
			result = result * isart::Mat4::CreateRotZ(rz);
			result = result * isart::Mat4::CreateRotX(rx);
			result = result * isart::Mat4::CreateRotY(ry);
			result = result * isart::Mat4::CreateTranslation(tx, ty, tz);


			isart::Mat4 trueResult = isart::Mat4::Identity;
			trueResult = trueResult.CreateTransform(tx, ty, tz,
													sx, sy, sz,
													rx, ry, rz);


			const size_t maxSize = 16;
			for (size_t index = 0; index < maxSize; ++index)
			{
				std::wstring message(std::to_wstring(index));
				Assert::AreEqual(result.GetRawValueAt(index), trueResult.GetRawValueAt(index), 0.00001f, message.c_str());
			}
		}

		TEST_METHOD(Negate)
		{
			float tabResult[16] = { -1.f, 0.f, 0.f, 0.f, 
									0.f, -1.f, 0.f, 0.f, 
									0.f, 0.f, -1.f, 0.f, 
									0.f, 0.f, 0.f, -1.f };
			isart::Mat4 result(tabResult);

			isart::Mat4 trueResult(isart::Mat4::Identity);

			Assert::AreEqual(isart::Mat4::Negate(result), trueResult);
		}

		TEST_METHOD(Negate_Translante)
		{
			float tabResult[16] = { -1.f, 0.f, 0.f, 1.f,
									0.f, -1.f, 0.f, 1.f,
									0.f, 0.f, -1.f, 1.f,
									0.f, 0.f, 0.f, -1.f };
			isart::Mat4 result(tabResult);

			isart::Mat4 trueResult(isart::Mat4::Identity);
			trueResult = trueResult * isart::Mat4::CreateTranslation(-1.f, -1.f, -1.f);

			Assert::AreEqual(isart::Mat4::Negate(result), trueResult);
		}

		TEST_METHOD(Projection)
		{

			float aspectRatio = 1920.f / 1080.f;		// Pour que les pixels soient carres
			float fov = utils::ToRadian(90.f);
			float d = 1.0f / tan(fov / 2.0f);		// Distance focale (oeil -> plan de projection)
			float range = 1.0f / (0.01f - 1000.f);	// normalisation en NDC de la profondeur

			float tabResult[16] = { d / aspectRatio, 0.0f, 0.0f, 0.0f,
									0.0f, d, 0.0f, 0.0f,
									0.0f, 0.0f, (0.01f + 1000.f) * range, -1.0f,
									0.0f, 0.0f, (0.01f * 1000.f * 2.0f) * range, 0.0f };
			isart::Mat4 result(tabResult);

			isart::Mat4 trueResult(isart::Mat4::Identity);
			trueResult = trueResult * isart::Mat4::TransposedProjMatrix(1920.f, 1080.f, 90.f, 1000.f, 0.01f);

			const size_t maxSize = 16;
			for (size_t index = 0; index < maxSize; ++index)
			{
				std::wstring message(std::to_wstring(index));
				Assert::AreEqual(result.GetRawValueAt(index), trueResult.GetRawValueAt(index), 0.00001f, message.c_str());
			}
		}



	};
#pragma endregion
}