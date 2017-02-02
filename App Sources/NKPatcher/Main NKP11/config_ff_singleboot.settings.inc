;;; Flicker Filter Patch (interlaced video only)
;;;
;;; Any call to set the flicker filter value will be
;;; replaced by a call with the value FLICKER_FILTER_VALUE.
;;;
;;; Takes values in the range 0 (off) to 5 (max filter).
;;;
%define FLICKER_FILTER_VALUE 2