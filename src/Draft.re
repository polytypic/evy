open Util;

module Position = {
  type t = {
    row: Int.t,
    columnt: Int.t,
  };
};

module Range = {
  type t = {
    path: String.t,
    start: Position.t,
    stop: Position.t,
  };
};

module Variable = {
  type t = String.t;
};

module Label = {
  type t = String.t;
};

module Cardinality = {
  type t =
    | Known(Int.t)
    | Unknown;

  let multiply = (lhs, rhs) =>
    switch (lhs, rhs) {
    | (Known(0), _)
    | (_, Known(0)) => Known(0)
    | (Known(lhsN), Known(rhsN)) => Known(lhsN * rhsN)
    | (_, _) => Unknown
    };
};

module Availability = {
  type t =
    | Synchronous
    | Asynchronous;
};

module Core = {
  module Labeled = {
    type t('expression) = (Label.t, 'expression);
  };

  module LabeledAt = Fork((K(Range)), Labeled);

  module TypeExpression = {
    type t('typeExpression) =
      | Unit
      | Number
      | String
      | Vector('typeExpression)
      | Record(Vector.t(LabeledAt.t('typeExpression)))
      | Function('typeExpression, 'typeExpression);
  };

  module TypeExpressionAt = Fix((Fork((K(Range)), TypeExpression)));

  module ValueBinding = {
    type t('expression) = (Variable.t, 'expression);
  };

  module ValueBindingAt = Fork((K(Range)), ValueBinding);

  module Expression = {
    type t('expression) =
      | Reference(Variable.t)
      | Funtion(Variable.t, 'expression)
      | Apply('expression, 'expression)
      // ...
      | Let(Vector.t(ValueBindingAt.t('expression)), 'expression)
      | If('expression, 'expression, 'expression)
      // ...
      | Unit
      | Number(Number.t)
      | String(String.t)
      | Vector(Vector.t('expression))
      | Record(Vector.t(LabeledAt.t('expression)));
  };

  module ExpressionAt =
    Fix((Fork((Fork((K(Range)), (K(TypeExpressionAt)))), Expression)));

  module Declaration = {
    type t =
      | Value(ValueBinding.t(ExpressionAt.t));
  };
};