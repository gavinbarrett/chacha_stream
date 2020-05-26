### CheChe Stream

This repo contains an implementation of a ChaCha20/Poly1305 AEAD encryption protocol. Daniel Bernstein's original [paper](https://cr.yp.to/chacha/chacha-20080128.pdf) defines a symmetric stream cipher that is very efficient and easy to implement in software. This cipher is commonly paired with Bernstein's [Poly1305 message authentication code](https://cr.yp.to/mac/poly1305-20050329.pdf) to construct an AEAD protocol that provides both data security (via the ChaCha20 cipher) as well as data integrity (via the Poly1305 MAC).
