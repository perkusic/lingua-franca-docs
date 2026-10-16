# Lingua Franca e HW-RTI — documentos

Duas frentes que compartilham uma base: os **documentos de Lingua Franca** (uma edição tutorial e uma de
consolidação técnica, com o mesmo conteúdo técnico) e a **especificação do HW-RTI**, uma implementação
sintetizável em Chisel/FPGA da lógica de decisão do RTI. A ponte entre elas é a §9.9 dos documentos de LF
(*What a hardware RTI would have to implement*). Corpo dos documentos em inglês.

Os números abaixo são de 15/out/2026; os exatos, medidos dos arquivos, estão em `lf-docs/INVENTARIO.md`, que
prevalece sobre este README e sobre os handoffs.

## `tutorial/` — documentos de Lingua Franca

| Arquivo | O que é |
| --- | --- |
| `lingua_franca_tutorial.html` | edição didática: 16 lições em 5 partes, objetivos, checkpoints e autoverificação (103 perguntas), 13 exemplos de código |
| `lingua_franca_rti.html` | edição de consolidação técnica: as mesmas 16 seções sem o aparato didático |
| `examples/` | fontes dos exemplos (20 `.lf` em C e Python, `fixpoint.py`, `micro-lf/`), com README |
| `figures/` | as 44 figuras extraídas dos HTMLs, esqueletos, manifesto e os scripts de extração e montagem; ver `figures/REPORT.md` |
| `HANDOFF_lingua_franca.md` | ponteiro para o handoff no projeto |

Estrutura comum: 16 seções (14 = ferramentas, 15 = apêndice de pontos fixos, 16 = micro-LF sem RTI), 48
subseções, 44 figuras SVG inline em `viewBox="0 0 760 H"`, 51 referências. Remissões ("section N.M",
"Lesson N" ou "section N", "Figure N", "Example N.M"; nunca "§") são links internos; toda subseção tem `id="sN-M"` e toda figura um `id` estável.
**Os dois HTMLs são a fonte canônica**; `figures/` é uma extração verificada, não a fonte.

## `hw-rti/` — especificação

`HW-RTI_project_specification.html` (revisão 0.2): Fase 1 on-chip, topologia estática, alvo n = 8 (até 32),
coordenação centralizada, plataforma ZCU106. 15 seções, 12 figuras em `viewBox="0 0 780 H"`, IDs
`FR/NFR/D/OQ/R/...`.

## Outras pastas

| Pasta | O que é |
| --- | --- |
| `micro-lf/` | nota de projeto micro-LF/FPGA (texto aceito) |
| `program/` | `implementation_guide.html`, guia único de decisões de implementação (a visão geral antiga é um stub que redireciona) |
| `lf-docs/` | `INVENTARIO.md` (inventário medido), `REVISAO_REGISTRO.md` (registro das revisões de redação), `linkrefs.py` |
| `governance/` | cópias dos documentos de governança do projeto: `INDEX.md` e os dois handoffs |

## Verificação

Cada push roda `.github/workflows/check.yml`: as figuras precisam reconstruir os dois HTMLs byte a byte
(`tutorial/figures/assemble.py --check`), e os HTMLs não podem ter ids duplicados, links internos sem destino
nem referências sem citação. Localmente:

    cd tutorial/figures && python3 assemble.py --check
    python3 .github/scripts/check_html.py tutorial/lingua_franca_tutorial.html tutorial/lingua_franca_rti.html

Depois de editar um HTML, rode `python3 extract.py ../lingua_franca_tutorial.html ../lingua_franca_rti.html` em
`tutorial/figures/` para atualizar a extração. Origem desta cópia e o que ficou fora: `REPOSITORIO.md`.
