; F20 - Toggle
; F22/F21 - Value +/-
; F24/F23 - Hue +/-

F20::Run cmd /c "echo e > COM9"
F22::Run cmd /c "echo v+ > COM9"
F21::Run cmd /c "echo v- > COM9"
F24::Run cmd /c "echo h+ > COM9"
F23::Run cmd /c "echo h- > COM9"
