; F20 - Toggle
; F22/F21 - Value +/-
; F24/F23 - Hue +/-

F20::Run cmd /c "echo e > COM9",,Hide
F22::Run cmd /c "echo v+ > COM9",,Hide
F21::Run cmd /c "echo v- > COM9",,Hide
F24::Run cmd /c "echo h+ > COM9",,Hide
F23::Run cmd /c "echo h- > COM9",,Hide
