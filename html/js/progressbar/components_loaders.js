$(function() {
    // Start
    $('#h-default-basic-start').click(function() {
        var $pb = $('#h-default-basic .progress-bar');
        $pb.attr('data-transitiongoal', $pb.attr('data-transitiongoal-backup'));
        $pb.progressbar();
    });

    // Reset
    $('#h-default-basic-reset').click(function() {
        $('#h-default-basic .progress-bar').attr('data-transitiongoal', 0).progressbar();
    });

});