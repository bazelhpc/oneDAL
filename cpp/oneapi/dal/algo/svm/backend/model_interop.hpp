/*******************************************************************************
* Copyright 2020 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#pragma once

#include "oneapi/dal/backend/interop/common.hpp"
#include "oneapi/dal/backend/interop/table_conversion.hpp"

#include <daal/include/algorithms/svm/svm_model.h>

namespace oneapi::dal::svm::backend {

namespace interop = dal::backend::interop;
namespace daal_svm = daal::algorithms::svm;

struct daal_model_builder : public daal::algorithms::svm::Model {
    daal_model_builder() = default;
    virtual ~daal_model_builder() {}

    auto& set_support_vectors(daal::data_management::NumericTablePtr support_vectors) {
        _SV = support_vectors;
        return *this;
    }

    auto& set_coeffs(daal::data_management::NumericTablePtr coeffs) {
        _SVCoeff = coeffs;
        return *this;
    }

    auto& set_bias(double bias) {
        _bias = bias;
        return *this;
    }
};

template <typename Task, typename Float>
inline auto convert_from_daal_model(daal_svm::Model& model) {
    auto table_support_vectors =
        interop::convert_from_daal_homogen_table<Float>(model.getSupportVectors());
    auto table_classification_coeffs =
        interop::convert_from_daal_homogen_table<Float>(model.getClassificationCoefficients());
    const double bias = model.getBias();

    return dal::svm::model<Task>()
        .set_support_vectors(table_support_vectors)
        .set_coeffs(table_classification_coeffs)
        .set_bias(bias);
}

} // namespace oneapi::dal::svm::backend
