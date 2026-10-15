# Sobre este repositório

Cópia, em 15/out/2026, da pasta `Cursos/Lingua Franca/tutoriais` do OneDrive. Os arquivos são
byte-idênticos aos da pasta: cada um foi conferido por SHA-256 antes do commit.

- **Fora do repositório:** os `.docx` vindos do ChatGPT (`micro-lf/micro_lf_fpga_design_note_rev1.docx`,
  `program/lf_hardware_coordination_zcu106_expanded.docx`) e `Claude outputs/` (cópia idêntica de
  `program/implementation_guide.html`).
- **Figuras:** `tutorial/figures/svg/` foi regenerado aqui com `extract.py` a partir dos HTMLs canônicos, e o
  `manifest.json` resultante é idêntico ao da pasta; `python3 tutorial/figures/assemble.py --check` dá PASS.
- **`.gitattributes` com `* -text`:** o git não converte fins de linha, o que mantém a reconstrução das figuras
  byte a byte em qualquer sistema operacional.
- Os HTMLs em `tutorial/` continuam sendo a fonte canônica; ver `tutorial/figures/REPORT.md`.
