#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() { 
	matWorld_ = MakeAfineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}