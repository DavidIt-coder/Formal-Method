From Stdlib Require Import List Arith Lia.
Theorem plus_zero : forall n : nat, n + 0 = n.
Proof.
  intros n.
  induction n; simpl; auto.
Qed.