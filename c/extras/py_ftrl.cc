//------------------------------------------------------------------------------
// Copyright 2018 H2O.ai
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//------------------------------------------------------------------------------
#include "extras/py_ftrl.h"
#include "frame/py_frame.h"
#include "python/float.h"
#include "python/int.h"
#include "python/tuple.h"

namespace py {

PKArgs PyFtrl::Type::args___init__(0, 0, 10, false, false,
                                 {"params", "a", "b", "l1", "l2", "d", "n_epochs",
                                 "inter", "hash_type", "seed"},
                                 "__init__", nullptr);

void PyFtrl::m__init__(PKArgs& args) {
  FtrlParams fp = Ftrl::fp_default;
  bool defined_params = !(args[0].is_undefined() || args[0].is_none());
  bool defined_a = !(args[1].is_undefined() || args[1].is_none());
  bool defined_b = !(args[2].is_undefined() || args[2].is_none());
  bool defined_l1 = !(args[3].is_undefined() || args[3].is_none());
  bool defined_l2 = !(args[4].is_undefined() || args[4].is_none());
  bool defined_d = !(args[5].is_undefined() || args[5].is_none());
  bool defined_n_epochs= !(args[6].is_undefined() || args[6].is_none());
  bool defined_inter = !(args[7].is_undefined() || args[7].is_none());

  if (defined_params) {
    if (!(defined_a || defined_b || defined_l1 || defined_l2
        || defined_d || defined_n_epochs || defined_inter)) {

      py::otuple arg0_tuple = args[0].to_pytuple();
      fp.a = arg0_tuple.get_attr("a").to_double();
      fp.b = arg0_tuple.get_attr("b").to_double();
      fp.l1 = arg0_tuple.get_attr("l1").to_double();
      fp.l2 = arg0_tuple.get_attr("l2").to_double();
      fp.d = static_cast<uint64_t>(arg0_tuple.get_attr("d").to_size_t());
      fp.n_epochs = arg0_tuple.get_attr("n_epochs").to_size_t();
      fp.inter = arg0_tuple.get_attr("inter").to_bool_strict();

    } else {
      throw TypeError() << "You can either pass all the parameters with `params` or "
            << " any of the individual parameters with `a`, `b`, `l1`, `l2`, `d`,"
            << "`n_epchs` or `inter` to Ftrl constructor, but not both at the same time";
    }
  } else {
    if (defined_a) {
      fp.a = args[1].to_double();
    }

    if (defined_b) {
      fp.b = args[2].to_double();
    }

    if (defined_l1) {
      fp.l1 = args[3].to_double();
    }

    if (defined_l2) {
      fp.l2 = args[4].to_double();
    }

    if (defined_d) {
      fp.d = static_cast<uint64_t>(args[5].to_size_t());
    }

    if (defined_n_epochs) {
      fp.n_epochs = args[6].to_size_t();
    }

    if (defined_inter) {
      fp.inter = args[7].to_bool_strict();
    }

    if (!(args[8].is_undefined() || args[8].is_none())) {
      fp.hash_type = static_cast<unsigned int>(args[8].to_size_t());
    }

    if (!(args[9].is_undefined() || args[9].is_none())) {
      fp.seed = static_cast<unsigned int>(args[9].to_size_t());
    }
  }

  ft = new Ftrl(fp);
}


void PyFtrl::m__dealloc__() {
  delete ft;
}


const char* PyFtrl::Type::classname() {
  return "datatable.core.Ftrl";
}


const char* PyFtrl::Type::classdoc() {
  return R"(Follow the Regularized Leader (FTRL) model with hashing trick.
    
See this reference for more details:
https://www.eecs.tufts.edu/~dsculley/papers/ad-click-prediction.pdf

Parameters
----------
a : float
    `alpha` in per-coordinate learning rate formula.
b : float
    `beta` in per-coordinate learning rate formula.
l1 : float
    L1 regularization parameter.
l2 : float
    L2 regularization parameter.
d : int
    Number of bins to be used after the hashing trick. 
n_epochs : int
    Number of epochs to train for.
inter : bool
    If feature interactions to be used or not.
hash_type : int
    Hashing method to use for strings:
    `0` - std::hash;
    `1` - Murmur2;
    `2` - Murmur3.
seed: integer
    Seed to be used for Murmur hash functions.
)";
}


void PyFtrl::Type::init_methods_and_getsets(Methods& mm, GetSetters& gs) {
  gs.add<&PyFtrl::get_model, &PyFtrl::set_model>("model",
    "Frame having two columns, i.e. `z` and `n`, and `d` rows,\n"
    "where `d` is a number of bins set for modeling. Both column types\n"
    "must be `FLOAT64`.\n");
  gs.add<&PyFtrl::get_params, &PyFtrl::set_params>("params", "FTRL model parameters.\n");
  gs.add<&PyFtrl::get_default_params>("default_params", "FTRL model default parameters.\n");

  gs.add<&PyFtrl::get_a, &PyFtrl::set_a>("a", "`alpha` in per-coordinate learning rate formula.\n");
  gs.add<&PyFtrl::get_b, &PyFtrl::set_b>("b", "`beta` in per-coordinate learning rate formula.\n");
  gs.add<&PyFtrl::get_l1, &PyFtrl::set_l1>("l1", "L1 regularization parameter.\n");
  gs.add<&PyFtrl::get_l2, &PyFtrl::set_l2>("l2", "L2 regularization parameter.\n");
  gs.add<&PyFtrl::get_d, &PyFtrl::set_d>("d", "Number of bins to be used after the hashing trick.\n");
  gs.add<&PyFtrl::get_n_epochs, &PyFtrl::set_n_epochs>("n_epochs", "Number of epochs to train for.\n");
  gs.add<&PyFtrl::get_inter, &PyFtrl::set_inter>("inter", "If feature interactions to be used or not.\n");
  gs.add<&PyFtrl::get_hash_type, &PyFtrl::set_hash_type>("hash_type", "Hashing method to use for strings.\n"
    "`0` - std::hash;\n"
    "`1` - Murmur2;\n"
    "`2` - Murmur3.\n");
  gs.add<&PyFtrl::get_seed, &PyFtrl::set_seed>("seed", "Seed to be used for Murmur hash functions.\n");

  mm.add<&PyFtrl::fit, args_fit>();
  mm.add<&PyFtrl::predict, args_predict>();
  mm.add<&PyFtrl::reset, args_reset>();
}


PKArgs PyFtrl::Type::args_fit(1, 0, 0, false, false, {"frame"}, "fit",
R"(fit(self, frame)
--

Train an FTRL model on a dataset.

Parameters
----------
frame: Frame
    Frame to be trained on, last column is treated as `target`.

Returns
----------
    None
)");


void PyFtrl::fit(const PKArgs& args) {
  DataTable* dt_train = args[0].to_frame();
  ft->fit(dt_train);
}


PKArgs PyFtrl::Type::args_predict(1, 0, 0, false, false, {"frame"}, "predict",
R"(predict(self, frame)
--

Make predictions for a dataset.

Parameters
----------
frame: Frame
    Frame of shape `(nrows, ncols)` to make predictions for. It must have one 
    column less than the training dataset.

Returns
----------
    A new `Frame` of shape `(nrows, 1)` with a prediction for each row.
)");


oobj PyFtrl::predict(const PKArgs& args) {
  if (ft->is_trained()) {
    DataTable* dt_test = args[0].to_frame();
    DataTable* dt_target = ft->predict(dt_test).release();
    py::oobj df_target = py::oobj::from_new_reference(py::Frame::from_datatable(dt_target));
    return df_target;
  } else {
    throw ValueError() << "Cannot make any predictions, because the model was not trained";
  }
}


PKArgs PyFtrl::Type::args_reset(0, 0, 0, false, false, {}, "reset",
R"(reset(self)
--

Reset an FTRL model.

Parameters
----------
    None

Returns
----------
    None
)");


void PyFtrl::reset(const PKArgs&) {
  ft->init_model();
}

/*
*  Getter and setter for the model datatable.
*/
oobj PyFtrl::get_model() const {
  if (ft->is_trained()) {
    DataTable* dt_model = ft->get_model();
    py::oobj df_model = py::oobj::from_new_reference(py::Frame::from_datatable(dt_model));
    return df_model;
  } else {
    return py::None();
  }
}


void PyFtrl::set_model(robj model) {
  DataTable* dt_model_in = model.to_frame();
  const std::vector<std::string>& model_cols_in = dt_model_in->get_names();

  if (dt_model_in->nrows != ft->get_d() || dt_model_in->ncols != 2) {
    throw ValueError() << "FTRL model frame must have " << ft->get_d() << " rows,"
                       << "and 2 columns, whereas your frame has " << dt_model_in->nrows
                       << " rows and " << dt_model_in->ncols << " columns";
  }

  if (model_cols_in != Ftrl::model_cols) {
    throw ValueError() << "FTRL model frame must have columns named `z` and `n`,"
                       << "whereas your frame has the following column names `" << model_cols_in[0]
                       << "` and `" << model_cols_in[1] << "`";
  }

  if (dt_model_in->columns[0]->stype() != SType::FLOAT64 ||
    dt_model_in->columns[1]->stype() != SType::FLOAT64) {
    throw ValueError() << "FTRL model frame must have both column types as `float64`, "
                       << "whereas your frame has the following column types: `"
                       << dt_model_in->columns[0]->stype()
                       << "` and `" << dt_model_in->columns[1]->stype() << "`";
  }

  ft->set_model(dt_model_in);
}


/*
*  All other getters and setters.
*/
oobj PyFtrl::get_params() const {
  py::otuple params(7);
  params.set(0, get_a());
  params.set(1, get_b());
  params.set(2, get_l1());
  params.set(3, get_l2());
  params.set(4, get_d());
  params.set(5, get_n_epochs());
  params.set(6, get_inter());
  return std::move(params);
}


void PyFtrl::set_params(robj params) {
  set_a(params.get_attr("a"));
  set_b(params.get_attr("b"));
  set_l1(params.get_attr("l1"));
  set_l2(params.get_attr("l2"));
  set_d(params.get_attr("d"));
  set_n_epochs(params.get_attr("n_epochs"));
  set_inter(params.get_attr("inter"));
  // TODO: check that there are no unknown parameters
}


oobj PyFtrl::get_default_params() const {
  py::otuple params(7);
  params.set(0, py::ofloat(Ftrl::fp_default.a));
  params.set(1, py::ofloat(Ftrl::fp_default.b));
  params.set(2, py::ofloat(Ftrl::fp_default.l1));
  params.set(3, py::ofloat(Ftrl::fp_default.l2));
  params.set(4, py::oint(static_cast<size_t>(Ftrl::fp_default.d)));
  params.set(5, py::oint(Ftrl::fp_default.n_epochs));
  params.set(6, py::oint(Ftrl::fp_default.inter));
  return std::move(params);
}


oobj PyFtrl::get_a() const {
  return py::ofloat(ft->get_a());
}


oobj PyFtrl::get_b() const {
  return py::ofloat(ft->get_b());
}


oobj PyFtrl::get_l1() const {
  return py::ofloat(ft->get_l1());
}


oobj PyFtrl::get_l2() const {
  return py::ofloat(ft->get_l2());
}


oobj PyFtrl::get_d() const {
  return py::oint(static_cast<size_t>(ft->get_d()));
}


oobj PyFtrl::get_n_epochs() const {
  return py::oint(ft->get_n_epochs());
}


oobj PyFtrl::get_inter() const {
  return py::oint(static_cast<size_t>(ft->get_inter()));
}


oobj PyFtrl::get_hash_type() const {
  return py::oint(static_cast<size_t>(ft->get_hash_type()));
}


oobj PyFtrl::get_seed() const {
  return py::oint(static_cast<size_t>(ft->get_seed()));
}


void PyFtrl::set_a(robj a) {
  if (!a.is_numeric()) {
    throw TypeError() << "`a` must be numeric, not "
        << a.typeobj();
  }
  ft->set_a(a.to_double());
}


void PyFtrl::set_b(robj b) {
  if (!b.is_numeric()) {
    throw TypeError() << "`b` must be numeric, not "
        << b.typeobj();
  }
  ft->set_b(b.to_double());
}


void PyFtrl::set_l1(robj l1) {
  if (!l1.is_numeric()) {
    throw TypeError() << "`l1` must be numeric, not "
        << l1.typeobj();
  }
  ft->set_l1(l1.to_double());
}


void PyFtrl::set_l2(robj l2) {
  if (!l2.is_numeric()) {
    throw TypeError() << "`l2` must be numeric, not "
        << l2.typeobj();
  }
  ft->set_l2(l2.to_double());
}


void PyFtrl::set_d(robj d) {
  if (!d.is_int()) {
    throw TypeError() << "`d` must be integer, not "
        << d.typeobj();
  }
  int64_t d_in = d.to_int64_strict();
  if (d_in < 0) {
    throw ValueError() << "`d` cannot be negative";
  }
  ft->set_d(static_cast<uint64_t>(d_in));
}


void PyFtrl::set_n_epochs(robj n_epochs) {
  if (!n_epochs.is_int()) {
    throw TypeError() << "`n_epochs` must be integer, not "
        << n_epochs.typeobj();
  }
  int64_t n_epochs_in = n_epochs.to_int64_strict();
  if (n_epochs_in < 0) {
    throw ValueError() << "`n_epochs` cannot be negative";
  }
  ft->set_n_epochs(static_cast<size_t>(n_epochs_in));
}


void PyFtrl::set_inter(robj inter) {
  if (!inter.is_bool()) {
    throw TypeError() << "`inter` must be boolean, not "
        << inter.typeobj();
  }
  bool inter_in = inter.to_bool();
  ft->set_inter(inter_in);
}


void PyFtrl::set_hash_type(robj hash_type) {
  if (!hash_type.is_int()) {
    throw TypeError() << "`hash_type` must be integer, not "
        << hash_type.typeobj();
  }
  int64_t hash_type_in = hash_type.to_int64_strict();
  if (hash_type_in != 0 && hash_type_in != 1 && hash_type_in !=2) {
    throw ValueError() << "`hash_type_in` must be either `0` or `1` or `2`";
  }
  ft->set_hash_type(static_cast<unsigned int>(hash_type_in));
}


void PyFtrl::set_seed(robj seed) {
  if (!seed.is_int()) {
    throw TypeError() << "`seed` must be integer, not "
        << seed.typeobj();
  }
  int32_t seed_in = seed.to_int32_strict();
  if (seed_in < 0) {
    throw ValueError() << "`seed` cannot be negative";
  }
  ft->set_seed(static_cast<unsigned int>(seed_in));
}

} // namespace py
