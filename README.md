# Quine-McCluskey
An implementation of the Quine-McCluskey algorithm to simpily a DNF written in C

## How it works:
1. Parse the input (the sum of products) into a format we can use, nested structs
2. Create the simplification table of minterms
3. Merge minterms if possible, marking them if merged
4. Keep merging until we can't any more
5. Get what we couldn't merge, the 'Prime Implicants'- this is the output

## Example
`./quineMcCluskey -C-B-A + -CB-A + C-B-A + CB-A + CBA`

`-A + CB`

(Alternative output format:)

`A v (C ∧ B)`
