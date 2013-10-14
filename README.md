lsch
====

lesser scheme (ore-ore minimum scheme)
SICP4章を読んで作りました。

機能
=====
* 基本的な演算子 +, -, *, =, if, else
* リスト list, null?, car, cdr, (cons?͂
* define lambda 。再帰動きます


コンパイルと実行
============

C++0xに対応したC++コンパイラ（auto しか使ってないが）。

$ make
$./lesh < sample.sch
$./lesh
(+ 1 2)
3
(* 2 3)
6
(define (f n ) (if (= n 1 ) 1 (* n (f (- n 1 ) ) ) ) )
ok
(f 10) 
3628800
(define (fib n ) (if (= n 0 ) 0 (if (= n 1 ) 1 (+ (fib (- n 1 ) ) (fib (- n 2 )  ) ) ) )
ok
(fib 10) 
55

TODO
=====
* パーサーが適当
* エラー処理がない
* 遅い（コピー、オブジェクト作りまくり）
* let, begin, cons 等サポート

