// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	struct STransform
	{
	private:
		SMatrix LocalMatrix = SMatrix::Identity;
		SMatrix WorldMatrix = SMatrix::Identity;
		STransform* Parent = nullptr;
		std::vector<STransform*> Children;

	public:
		[[nodiscard]] SMatrix& GetMatrix();
		void SetParent(STransform* parent);
		void SetChild(STransform* child);
		void RemoveChild(STransform* child);
		void ClearChildren();

		inline void Rotate(const SMatrix& rotationMatrix);
		inline void Rotate(const SVector& eulerAngles);
		inline void Translate(const SVector& v);
		inline void Translate(const SVector4& v);
		inline void Orbit(const STransform& transform, const SMatrix& rotation);
		inline void Orbit(const SVector& point, const SMatrix& rotation);
		inline void Orbit(const SVector4& point, const SMatrix& rotation);
	};

	inline SMatrix& STransform::GetMatrix()
	{
		if (Parent)
		{
			WorldMatrix = LocalMatrix * Parent->GetMatrix();
			return WorldMatrix;
		}

		return LocalMatrix;
	}

	inline void STransform::SetParent(STransform* parent)
	{
		Parent = parent;
	}

	inline void STransform::SetChild(STransform* child)
	{
		Children.emplace_back(child);
	}

	inline void STransform::RemoveChild(STransform* child)
	{
		Children.erase(std::ranges::find(Children, child));
	}

	inline void STransform::ClearChildren()
	{
		Children.clear();
	}

	// TODO.NR: Fix this
	inline void STransform::Rotate(const SMatrix& rotationMatrix)
	{
		SMatrix copy = rotationMatrix;
		copy *= LocalMatrix.GetRotationMatrix();
		LocalMatrix.SetRotation(LocalMatrix.GetRotationMatrix() * copy);
	}

	inline void STransform::Rotate(const SVector& eulerAngles)
	{
		if (eulerAngles.IsEqual(SVector::Zero))
			return;

		const SMatrix rightRotation = SMatrix::CreateRotationAroundAxis(eulerAngles.X, LocalMatrix.Right());
		const SMatrix upRotation = SMatrix::CreateRotationAroundAxis(eulerAngles.Y, LocalMatrix.Up());
		const SMatrix forwardRotation = SMatrix::CreateRotationAroundAxis(eulerAngles.Z, LocalMatrix.Forward());
		SMatrix finalRotation = LocalMatrix.GetRotationMatrix();
		finalRotation *= rightRotation;
		finalRotation *= upRotation;
		finalRotation *= forwardRotation;
		LocalMatrix.SetRotation(finalRotation);
	}

	inline void STransform::Translate(const SVector& v)
	{
		SVector localMove = LocalMatrix.Right() * v.X;
		LocalMatrix.M[3][0] += localMove.X;
		LocalMatrix.M[3][1] += localMove.Y;
		LocalMatrix.M[3][2] += localMove.Z;
		localMove = LocalMatrix.Up() * v.Y;
		LocalMatrix.M[3][0] += localMove.X;
		LocalMatrix.M[3][1] += localMove.Y;
		LocalMatrix.M[3][2] += localMove.Z;
		localMove = LocalMatrix.Forward() * v.Z;
		LocalMatrix.M[3][0] += localMove.X;
		LocalMatrix.M[3][1] += localMove.Y;
		LocalMatrix.M[3][2] += localMove.Z;
	}

	inline void STransform::Translate(const SVector4& v)
	{
		SVector localMove = LocalMatrix.Right() * v.X;
		LocalMatrix.M[3][0] += localMove.X;
		LocalMatrix.M[3][1] += localMove.Y;
		LocalMatrix.M[3][2] += localMove.Z;
		localMove = LocalMatrix.Up() * v.Y;
		LocalMatrix.M[3][0] += localMove.X;
		LocalMatrix.M[3][1] += localMove.Y;
		LocalMatrix.M[3][2] += localMove.Z;
		localMove = LocalMatrix.Forward() * v.Z;
		LocalMatrix.M[3][0] += localMove.X;
		LocalMatrix.M[3][1] += localMove.Y;
		LocalMatrix.M[3][2] += localMove.Z;
	}

	// TODO.NR: Make transform struct which can store local transform data and make parent from point argument
	inline void STransform::Orbit(const STransform& /*transform*/, const SMatrix& rotation)
	{
		SMatrix finalRotation = rotation;
		//finalRotation.Translation(point);
		//SMatrix parentTransform = SMatrix();
		//parentTransform.Translation(point);
		//Translate(-point);
		LocalMatrix *= finalRotation;
		//(*this) *= parentTransform;
		//Translate(point);
	}

	// TODO.NR: Make transform struct which can store local transform data and make parent from point argument
	inline void STransform::Orbit(const SVector& /*point*/, const SMatrix& rotation)
	{
		SMatrix finalRotation = rotation;
		//finalRotation.Translation(point);
		//SMatrix parentTransform = SMatrix();
		//parentTransform.Translation(point);
		//Translate(-point);
		LocalMatrix *= finalRotation;
		//(*this) *= parentTransform;
		//Translate(point);
	}

	inline void STransform::Orbit(const SVector4& /*point*/, const SMatrix& rotation)
	{
		SMatrix finalRotation = rotation;
		//finalRotation.Translation(point);
		LocalMatrix *= finalRotation;
	}

}
