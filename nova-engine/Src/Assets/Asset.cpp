#include "Asset.h"

void Asset::AddRef() {
	++ref_count;
}

void Asset::Release() {
	if (--ref_count == 0) {
		OnDestroy();
		delete this;
	}
}

int Asset::GetReferenceCount() const {
	return ref_count;
}
