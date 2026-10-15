# tutoriais — organização

Reorganizado em 2026-09-18; arquivos obsoletos da raiz, `chatgpt/`,
`Claude outputs/` e `_antigo/` apagados em 2026-09-28. O material está separado
em duas pastas:

## `tutorial/`

Os documentos de Lingua Franca — as duas edições irmãs do mesmo material,
com 95,4% de texto em comum, governadas por
`claude/HANDOFF_lingua_franca_docs.md` no projeto **LinguaFranca**.

| Arquivo | O que é |
| --- | --- |
| `lingua_franca_tutorial.html` | edição didática (lições, checkpoints, lab steps) |
| `lingua_franca_rti.html` | edição de consolidação técnica |
| `HANDOFF_lingua_franca.md` | ponteiro para o handoff do projeto |

Convenções: 13 seções numeradas, 13 figuras em `viewBox="0 0 760 H"`,
23 referências.

## `hw-rti/`

A especificação do projeto de hardware, governada por
`claude/HANDOFF_hw_rti_spec.md` no projeto.

| Arquivo | O que é |
| --- | --- |
| `HW-RTI_project_specification.html` | a especificação |
| `HANDOFF.md` | ponteiro para o handoff do projeto |

Convenções: 15 seções, 6 figuras em `viewBox="0 0 780 H"`,
IDs `FR-01..19`, `NFR-01..09`, `D-1..10`, `OQ-1..6`, `R-1..8`.

## `lf-docs/`

`INVENTARIO.md` — inventário estrutural dos três HTMLs, medido a partir dos
arquivos, cobrindo as duas pastas. Use como base de conferência contra os
handoffs do projeto.

## A ponte entre as duas

A §9.9 dos documentos de LF — *What a hardware RTI would have to implement* —
é o que leva à especificação.
