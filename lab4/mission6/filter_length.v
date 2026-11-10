From Stdlib Require Import List Arith Lia.
Import ListNotations.
Theorem filter_length_le :
  forall (A : Type) (keep : A -> bool) (xs : list A),
    length (filter keep xs) <= length xs.
Proof.
  intros A keep xs.
  induction xs as [| x xs IH].
  - (* 空列表情形 *)
    simpl.
    lia.
  - (* 非空列表情形 *)
    simpl.
    destruct (keep x) eqn:Hkeep.
    + (* keep x = true, 保留元素*)
        simpl.
        lia.
    + (* keep x = false, 不保留元素 *)
        simpl.
        lia.
Qed.

Theorem filter_length_le_short :
  forall (A : Type) (keep : A -> bool) (xs : list A),
    length (filter keep xs) <= length xs.
Proof.
  intros A keep xs.
  induction xs as [| x xs IH]; simpl.
  - lia.
  - destruct (keep x); simpl; lia.
Qed.