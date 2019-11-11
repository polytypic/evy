module type Tycon0 = {type t;};
module type Tycon1 = {type t('value);};

module type Functor = {
  include Tycon1;
  let map: ('t => 'u, t('t)) => t('u);
};

module type Pointed = {
  include Functor;
  let pure: 't => t('t);
};

module type Applicative = {
  include Pointed;
  let ap: (t('t => 'u), t('t)) => t('u);
};

//

module K = (T: Tycon0) => {
  type t('t) = T.t;
};

module Fork = (Lhs: Tycon1, Rhs: Tycon1) => {
  type t('t) = (Lhs.t('t), Rhs.t('t));
};

module Fix = (Tycon1: Tycon1) => {
  type t =
    | Fix(Tycon1.t(t));
};

module O = (F: Tycon1, G: Tycon1) => {
  type t('t) = F.t(G.t('t));
};

//

module Array = {
  include Array;
  type t('value) = array('value);
};

module type Vector = {
  type t('value);
  let fromArray: Array.t('v) => t('v);
  let map: ('t => 'u, t('t)) => t('u);
};

module Vector: Vector = {
  include Array;
  let fromArray = Array.copy;
};

module Int = {
  type t = int;
};

module Number = {
  type t = float;
};

module Option = {
  type t('value) = option('value);
};