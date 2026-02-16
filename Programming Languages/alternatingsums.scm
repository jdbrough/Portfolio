#lang scheme
; alternatingsums.scm
; Jonathan Brough
; 2025-04-23
;
; File for Assignment 7, Exercise B

(define (alternatingSums . args)
  (cond
    [(null? args)      '(0 0)]
    [else              (helper args 0 0 0)]))

(define (helper lst idx evenSum oddSum)
  (cond
    [(null? lst)        (list evenSum oddSum)]
    [(even? idx)        (helper (cdr lst) (+ idx 1) (+ evenSum (car lst)) oddSum)]
    [else               (helper (cdr lst) (+ idx 1) evenSum (+ oddSum (car lst)))]))