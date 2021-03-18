# Aim
I write this file to remind me what I did in the past.
I used a few list files to calculate significance. Now I want
to record them here. They are divided into two classes,
prompt D0 and LambdaC. Unfortunately, some of files have
been deleted from EOS area.
## Prompt D0
Three list file for HYDJET samples here.
  - `hyjets_06Dec2020.list`. I reproduced this one via reproducing
    the histograms. I checked the entries of `hMassVsPtVsY`. They
    have the same number of entries, between the one I used calculating
    significance during TDR on bonner, and the one I reproduced on lxplus.
    I also checked the number of events via `readEvts.C`. They give
    the same event number record.
    ```
    MB: 673
    central: 96
    30 - 50 132
    ```
    This one is used by default.
  - `newHyJets.list`.
    I checked this list via `readEvts.C`. This list gives the output
    which is same as the record in `myAna.h` on bonner. It is a list
    having 4799 events.
  - `newHyJets_2500.list`
    This one is half of the above one. Based on the record on bonner,
    `myAna.h`, it was used during July 2019, for MTD smear studies.

One merged file is used for reconstructed prompt D0 samples, under
`~work/MTDStudies/MTDStudies/HF-02Dec-2020/PromptD0/matchPromptD0Tree_fullSample_reRECO.root`

One list is used for generated prompt D0 samples, `newD0Signal.list`

With above materials, TDR results can be reproduced.

This section is written on
Mon Dec  7 04:57:56 CET 2020

## LambdaC
Signal without MTD cannot be reproduced. Check and use the attached root file.

Signal with MTD can be reproduced using `list` and `fillMatchTree.C`. You can use
either `LamCSignal.list` or `lamc3psignal_new.list`.

This section is written on
Fri Jan  1 04:59:11 CET 2021

I find some list and put them under `LamC3P`.

This section is written on
Thu Mar 18 20:23:02 CET 2021
