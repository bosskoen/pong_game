#pragma once

/// <summary>
/// Macro to make a class cloneable by overriding Clone() using dynamic_cast.
/// </summary>
/// <param name="DerivedType">The type that implements the clone.</param>
/// <param name="BaseType">The base interface type with virtual Clone().</param>
#define MAKE_CLONEABLE(DerivedType, BaseType) \
    BaseType* Clone() const override { return new DerivedType(dynamic_cast<const DerivedType&>(*this)); }

namespace Util {

	/// <summary>
	/// Interface for cloneable types. Provides a virtual Clone method.
	/// </summary>
	/// <typeparam name="Base">The base type to return from Clone().</typeparam>
	template<typename Base>
	class ICloneable {
	public:
		/// <summary>
	/// Creates a deep copy of the object.
	/// </summary>
	/// <returns>Pointer to a new copy of the base type.</returns>
		virtual Base* Clone() const = 0;
		virtual ~ICloneable() = default;
	};

    /// <summary>
    /// Helper base class to make a type cloneable.
    /// Inherits from Base to ensure Derived can be cast to a Base pointer.
    /// Also implements ICloneable<Base>.
    /// </summary>
    /// <typeparam name="Derived">The derived class implementing the clone.</typeparam>
    /// <typeparam name="Base">The base class used for the clone interface and casting.</typeparam>
	template<typename Derived, typename Base>
	class Cloneable : public virtual Base, public virtual ICloneable<Base> {
	public:
		/// <summary>
	/// Creates a deep copy using the derived class copy constructor.
	/// </summary>
	/// <returns>Pointer to a new copy of the base type.</returns>
		Base* Clone() const override {
			return new Derived(dynamic_cast<const Derived&>(*this));
		}
		virtual ~Cloneable() = default;
	};
}